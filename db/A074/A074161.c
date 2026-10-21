/*
 * A074161  Smallest multiple of n (>n) using all composite digits (4,6,8,9
 *          and 0).
 * https://oeis.org/A074161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   composite_digits_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS A074161 sequence:
 *  a(n) = smallest multiple of n (> n) that uses only composite digits
 *  {0,4,6,8,9}.  The algorithm scans successive multiples of n until it
 *  finds one whose decimal representation contains only these digits.
 */

#include <stdbool.h>
#include "../utils/composite_digits_ok.h"

/* a(idx_t n):  compute the smallest multiple of n (> n) that uses only
 * composite digits (0,4,6,8,9). */
seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * 2;        /* first multiple strictly greater than n */
    while (true) {
        if (composite_digits_ok(m))
            return m;
        m += (seq_t)n;              /* next multiple of n */
    }
}
