/*
 * A185298  Expansion of e.g.f. x*exp(x)*exp(x*exp(x)).
 * https://oeis.org/A185298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k) * k^(n-k+1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 37,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t sum   = 0;
    seq_t binom = 1;          /* C(n,0) = 1 */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update binomial coefficient C(n,k) */
        binom = binom * (seq_t)(n - k + 1) / (seq_t)k;

        /* Compute k^(n-k+1) using seq_pow */
        seq_t power = seq_pow((seq_t)k, n - k + 1);

        /* Accumulate term */
        sum += binom * power;
    }

    return sum;
}
