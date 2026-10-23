/*
 * A127670  Discriminants of Chebyshev S-polynomials A049310.
 * https://oeis.org/A127670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+1)^(n-2))*2^n, n >= 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   pow2, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        /* a(1) = 1 by definition */
        return (seq_t)1;
    }

    /* compute (n+1)^(n-2) */
    seq_t part = seq_pow((seq_t)(n + 1), (idx_t)(n - 2));

    /* compute 2^n */
    seq_t pow2v = pow2((idx_t)n);

    /* multiply the two parts */
    return part * pow2v;
}
