/*
 * A039540  Numbers whose base-12 representation has the same nonzero number
 *          of 5's and 8's.
 * https://oeis.org/A039540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   equal_fives_eights_base12
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
   Sequence A039540: Numbers whose base-12 representation has the same nonzero
   number of 5's and 8's.

   The algorithm scans the natural numbers in increasing order, checking
   for each whether the count of digit 5 equals the count of digit 8 (both
   non‑zero).  It returns the nth such number.  This direct search is
   efficient because the density of the sequence is about 3%, so for
   n≤10000 the required value is below a few hundred thousand and a
   linear scan easily fits within the time limits.
*/

#include <stdbool.h>
#include "../utils/equal_fives_eights_base12.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;          /* undefined for n=0, return 0 as a fallback */
    }

    idx_t found = 0;
    seq_t candidate = 1;

    while (1) {
        if (equal_fives_eights_base12(candidate)) {
            found++;
            if (found == n) {
                return candidate;
            }
        }
        candidate++;
    }
}
