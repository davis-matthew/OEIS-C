/*
 * A162891  Expansion of 1 / Product_{k>=1} (1-x^k-x^(2*k)).
 * https://oeis.org/A162891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in 1 / ∏_{k≥1} (1 - x^k - x^{2k})
 *            Recurrence used (derived from 1/(1 - q - q^2) = Σ_{m≥0} F_{m+1}
 *            q^m and the identity F_{m+1} = F_m + F_{m-1}): For each k = 1 ..
 *            n do, in place a_new[j] = a_old[j] + a_new[j-k] + a_new[j-2k] (j
 *            ≥ k) with a_new[j] for j < k staying unchan...
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate workspace */
    size_t size = (size_t)(n + 1);
    seq_t *dp = (seq_t *)malloc(size * sizeof(seq_t));
    if (!dp) {
        /* On memory allocation failure return 0 */
        return 0;
    }

    /* Initialize */
    dp[0] = 1;
    for (size_t i = 1; i < size; ++i) {
        dp[i] = 0;
    }

    /* Main DP: multiply by each factor 1/(1 - x^k - x^{2k}) in turn */
    for (idx_t k = 1; k <= n; ++k) {
        /* j runs from k to n (indices < k unchanged) */
        for (idx_t j = k; j <= n; ++j) {
            seq_t a = dp[j];
            seq_t b = dp[j - k];
            seq_t c = (j >= 2 * k) ? dp[j - 2 * k] : 0;
            dp[j] = a + b + c;
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
