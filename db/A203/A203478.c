/*
 * A203478  a(n) = v(n+1)/v(n), where v = A203477.
 * https://oeis.org/A203478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{j=0..n-1} (2^j + 2^n) (OEIS A203478)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* OEIS offset 1: the first term is a(1).  For n < 1 we return 0. */
    if (n < 1)
        return 0;

    seq_t twon = pow2(n);   /* 2^n */
    seq_t cur  = 1;         /* 2^0 at start */
    seq_t prod = 1;

    for (idx_t j = 0; j < n; ++j) {
        prod *= (cur + twon); /* multiply by (2^j + 2^n) */
        cur <<= 1;            /* next power of two: cur = cur * 2 */
    }

    return prod;
}
