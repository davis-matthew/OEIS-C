/*
 * A190999  a(n) = 2^(n^2)*(2^(2*n+1) - 1).
 * https://oeis.org/A190999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n^2) * (2^(2*n+1) - 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 2^(n^2) */
    idx_t sq = n * n;
    seq_t pow_sq = pow2(sq);

    /* 2^(2*n+1) - 1 */
    idx_t exp = 2 * n + 1;
    seq_t pow_exp = pow2(exp);
    seq_t diff = pow_exp - 1;

    return pow_sq * diff;
}
