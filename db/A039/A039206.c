/*
 * A039206  Numbers whose base-11 representation has the same number of 7's
 *          and 10's.
 * https://oeis.org/A039206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   has_equal_7_and_10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th integer whose base‑11 representation contains
   an equal number of digits 7 and 10.  The sequence starts with
   0, 1, 2, 3, 4, 5, 6, 8, 9, 11, … as in OEIS A039206.  */
#include <stdbool.h>
#include "../utils/has_equal_7_and_10.h"

seq_t a(idx_t n)
{
    idx_t  idx = 0;          /* how many terms found so far   */
    seq_t  val = 0;          /* current integer to test       */

    while (true) {
        if (has_equal_7_and_10(val)) {
            ++idx;
            if (idx == n)    /* n is 1‑based */
                return val;
        }
        /* Move to the next integer.  */
        val = val + 1;
    }
}
