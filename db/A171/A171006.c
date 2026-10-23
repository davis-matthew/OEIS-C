/*
 * A171006  Binomial(n-k-1,k) * binomial(n-k,k+1) where k = ceiling(n/4).
 * https://oeis.org/A171006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n-k-1,k) * binomial(n-k, k+1) where k =
 *            ceil(n/4)
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* compute k = ceil(n/4) */
    idx_t k = (n + 3) / 4;

    /* for n < k+1 the first binomial is undefined, result is zero */
    if (n < k + 1)
        return 0;

    seq_t b1 = binom(n - k - 1, k);     /* binomial(n-k-1, k) */
    seq_t b2 = binom(n - k, k + 1);     /* binomial(n-k,   k+1) */

    return b1 * b2;
}
