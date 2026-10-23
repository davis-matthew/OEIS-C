/*
 * A169937  a(n) = binomial(m+n-1,n)^2 - binomial(m+n,n+1)*binomial(m+n-2,n-1)
 *          with m = 14.
 * https://oeis.org/A169937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(14 + n - 1, n)^2 - binomial(14 + n, n +
 *            1)*binomial(14 + n - 2, n - 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 30, u128 n <= 230,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    static const idx_t m = 14;
    seq_t b1 = binom(m + n - 1, n);
    seq_t b2 = binom(m + n, n + 1);
    seq_t b3 = (n == 0) ? 0 : binom(m + n - 2, n - 1);
    return b1 * b1 - b2 * b3;
}
