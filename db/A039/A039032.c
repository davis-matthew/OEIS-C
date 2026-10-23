/*
 * A039032  Numbers whose base-7 representation has the same number of 0's and
 *          3's.
 * https://oeis.org/A039032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 125, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_equal_zero_three
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth natural number whose base‑7 representation has
   an equal number of 0's and 3's.  The function scans the natural numbers
   sequentially, counting such numbers until the n‑th is found. */
#include <stdbool.h>
#include "../utils/has_equal_zero_three.h"

seq_t a(idx_t n)
{
    idx_t found = 0;
    for (seq_t val = 1; ; ++val) {
        if (has_equal_zero_three(val)) {
            ++found;
            if (found == n) return val;
        }
    }
}
