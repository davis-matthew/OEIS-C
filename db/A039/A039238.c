/*
 * A039238  Numbers whose base-12 representation has the same number of 2's
 *          and 11's.
 * https://oeis.org/A039238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n-th nonnegative integer whose base‑12 representation has
 *            the same number of 2's as 11's
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   equal_2_and_11
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/equal_2_and_11.h"

seq_t a(idx_t n)
{
    /* The offset of the sequence is 1 (a(1) = 0).  n is guaranteed
       to be >= 1. */
    idx_t found = 0;
    seq_t val = 0;

    while (1) {
        if (equal_2_and_11(val)) {
            ++found;
            if (found == n)
                return val;
        }
        ++val;
    }
}
