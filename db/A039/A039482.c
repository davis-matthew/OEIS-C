/*
 * A039482  Numbers whose base-11 representation has the same nonzero number
 *          of 5's and 10's.
 * https://oeis.org/A039482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   has_equal_5_and_10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers whose base‑11 representation has equal nonzero
   numbers of digits 5 and 10. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_equal_5_and_10.h"

/* Public function required by the judge. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    seq_t value = 1;          /* start from the first positive integer */
    idx_t found  = 0;         /* how many terms have been found so far */

    while (found < n) {
        if (has_equal_5_and_10(value)) {
            found++;
            if (found == n) {
                break;
            }
        }
        value++;              /* move to the next integer */
    }
    return value;
}
