/*
 * A033736  Expansion of Product_{d | 24} theta_3(q^d).
 * https://oeis.org/A033736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of qⁿ in ∏_{d|24} θ₃(qᵈ), where
 *            θ₃(q)=∑_{k∈ℤ} q^{k²}. Equivalently, a(n) = coefficient of qⁿ in
 *            the product, obtained by successive convolution of the series
 *            θ₃(qᵈ)=1+2∑_{k>0} q^{d·k²} for all divisors d of 24
 * Range:     u8 n <= 20, u16 n <= 125, u32 n <= 4991, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Divisors of 24 in ascending order. */
    static const idx_t divs[8] = {1, 2, 3, 4, 6, 8, 12, 24};

    /* Special case n = 0 */
    if (n == 0)
        return (seq_t)1;

    /* Allocate array for dynamic programming, size n+1.  All entries
       are zeroed by calloc.  The initial array represents the
       product of an empty set of factors (coefficient 1 at 0).   */
    seq_t *dp = (seq_t *)calloc(n + 1, sizeof(seq_t));
    if (!dp) /* allocation failed; return 0 as fallback */
        return 0;
    dp[0] = (seq_t)1;

    /* Successively convolve with the series of each divisor. */
    for (size_t di = 0; di < 8; ++di) {
        idx_t d = divs[di];

        /* Temporary array for the next convolution stage. */
        seq_t *next = (seq_t *)calloc(n + 1, sizeof(seq_t));
        if (!next) {
            free(dp);
            return 0;
        }

        for (idx_t m = 0; m <= n; ++m) {
            seq_t sum = 0;
            for (idx_t k = 0; ; ++k) {
                /* Compute d·k²; stop when it exceeds m. */
                idx_t power = d * k * k;
                if (power > m)
                    break;

                seq_t add = dp[m - power];
                if (k > 0)            /* ±k contributes a factor 2 */
                    add *= (seq_t)2;
                sum += add;
            }
            next[m] = sum;
        }

        free(dp);
        dp = next;
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
