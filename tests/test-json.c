/* Copyright (c) 2017 Thorsten Kukuk
   Author: Thorsten Kukuk <kukuk@suse.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation in version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, see <http://www.gnu.org/licenses/>. */

#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <json-c/json.h>

#include "lock-json.h"

int
main (void)
{
  const char *input = "{\n    \"max\": 2,\n    \"holders\": [\"A\"]\n}";
  int64_t max;
  int64_t curr;

  printf("JSON string: %s\n", input);

  json_object * jobj = json_tokener_parse (input);

  max = get_max_locks (jobj);
  printf ("Max locks: %li\n", max);
  curr = get_curr_locks (jobj);
  printf ("Current number of locks: %li\n", curr);

  if (curr < max)
    {
      add_id_to_holders (jobj, "B");
    }

  printf ("The json object created:\n%s\n",
	  json_object_to_json_string_ext (jobj,JSON_C_TO_STRING_PRETTY));

  remove_id_from_holders (jobj, "A");

  if (is_id_in_holders (jobj, "A"))
    fprintf (stderr, "\"A\" found in holders, should have been removed!\n");

  if (!is_id_in_holders (jobj, "B"))
    fprintf (stderr, "\"B\" not found in holders, should have been added!\n");

  printf ("The json object created:\n%s\n",
	  json_object_to_json_string_ext (jobj,JSON_C_TO_STRING_PRETTY));

  if (json_object_put (jobj) != 1)
    fprintf (stderr, "json object not free'd!\n");

  return 0;
}
