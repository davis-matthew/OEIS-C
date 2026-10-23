/*
 * A143241  Expansion of Product_{k>0} (1 - x^k) / (1 - x^(6*k - 2)).
 * https://oeis.org/A143241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of the infinite product ∏_{k>0, k≢4 (mod 6)}
 *            (1 - x^k) . The value can be obtained by standard 0‑1 knapsack
 *            DP with an allowed part set consisting of all positive integers
 *            except those congruent to 4 modulo 6
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* allocate array for coefficients up to n (include index 0) */
    size_t N = (size_t)n;
    seq_t *dp = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!dp) return 0;               /* out of memory; return 0 safely */

    dp[0] = 1;                        /* constant term */

    /* 0‑1 knapsack: for each allowed part k, update descending */
    for (idx_t k = 1; k <= N; ++k)
    {
        if (k % 6 == 4)               /* skip parts congruent to 4 mod 6 */
            continue;

        for (idx_t i = N; i >= k; --i)
            dp[i] -= dp[i - k];
    }

    seq_t result = dp[N];
    free(dp);
    return result;
}
