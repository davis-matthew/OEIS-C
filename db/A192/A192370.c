/*
 * A192370  Sum of all the n-digit numbers whose digits are all even and
 *          nonzero: 2,4,6,8.
 * https://oeis.org/A192370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (5 * 4^n * (10^n-1))/9 (equivalently a(n) = 5 * 4^n *
 *            ((10^n-1)/9) ) where n ≥ 1 (the OEIS offset is 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) is not part of the sequence; return 0 for safety */
    if (n == 0) return 0;

    seq_t repunit = 0;   /* (10^n-1)/9 = 111...1 (n digits) */
    seq_t pow4   = 1;    /* 4^n */

    for (idx_t i = 0; i < n; ++i) {
        repunit = repunit * 10 + 1;   /* build the repunit */
        pow4    = pow4 * 4;           /* compute 4^n */
    }

    seq_t result = pow4 * repunit;   /* 4^n * repunit */
    result = result * 5;             /* multiply by 5 */
    return result;
}
