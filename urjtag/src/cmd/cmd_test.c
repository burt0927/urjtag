/*
 * $Id: test.c,v 1.0 2005/10/10 00:00:0 DJF $
 *
 * Copyright (C) 2005 Protoparts
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 *
 * Written by David Farrell, 2005
 * based on templates by and portions  Written by Marcel Telka <marcel@telka.sk>, 2003.i
 *
 */

#include <urjtag/sysdep.h>

#include <stdio.h>
#include <string.h>
//#include <stdlib.h>

#include <urjtag/chain.h>
#include <urjtag/part.h>
#include <urjtag/bssignal.h>

#include <urjtag/cmd.h>

static int
cmd_test_run (urj_chain_t *chain, char *params[])
{
    int data;
    unsigned int i;
    urj_part_signal_t *s;
    urj_part_t *part;

    if (urj_cmd_params (params) != 4)
        return -1;

    if (strcasecmp (params[1], "signal") != 0)
        return -1;

    if (!urj_cmd_test_cable (chain))
        return 1;

    part = urj_tap_chain_active_part (chain);
    if (part == NULL)
        return 1;

    s = urj_part_find_signal (part, params[2]);
    if (!s)
    {
        printf (_("signal '%s' not found\n"), params[2]);
        return 1;
    }

    /* values 0,1,X since X is not a number, the following failure exits clean
     * and doesnt test anything, as it should.
     */
    if (urj_cmd_get_number (params[3], &i))
        return 1;

    data = urj_part_get_signal (part, s);
    if (data != -1)
    {
        if (data != i)
        {
            printf (_("<FAIL>%s = %d\n"), params[2], data);
            return -99;
        }
    }
    return 1;
}

static void
cmd_test_help (void)
{
    printf (_("Usage: %s SIGNAL 0/1\n"
              "Test signal state from output BSR (Boundary Scan Register).\n"
              "\n"
              "SIGNAL        signal name (from JTAG declaration file)\n"),
            "get signal");
}

urj_cmd_t urj_cmd_test = {
    "test",
    N_("test external signal value"),
    cmd_test_help,
    cmd_test_run
};
