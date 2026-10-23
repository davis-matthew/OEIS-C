/*
 * A134485  Row sums of triangle A134484(n,k) = 2^[n(n-1) - k(k-1)] * C(n,k).
 * https://oeis.org/A134485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{n} 2^{n(n−1)−k(k−1)} * C(n,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* n(n−1) is used as the base of the exponent for k = 0 */
    idx_t base = n * (n - 1);      /* fits in uint64_t for all n ≤ 10000 */

    seq_t sum   = 0;               /* running total for the row sum */
    seq_t binom = 1;               /* C(n,0) = 1 */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* exponent for 2^... */
        idx_t exp = base - k * (k - 1);

        /* term = 2^exp * C(n,k) */
        seq_t term = big_pow((seq_t)2, (seq_t)exp) * binom;
        sum += term;

        /* prepare binomial coefficient for next k */
        if (k == n) break;
        binom = binom * (n - k) / (k + 1);
    }

    return sum;
}
