/*
 * A029208  Expansion of 1/((1-x^2)*(1-x^5)*(1-x^9)*(1-x^10)).
 * https://oeis.org/A029208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 2, 5, 9, 10 *
 *            Computation: classic unbounded‑knapsack (coin change) DP
 * Range:     u8 n <= 98, u16 n <= 694, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Create a fresh DP table for this call */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;                /* allocation failure */

    /* Base case: one way to form sum 0 – use no coins */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = (seq_t)0;

    /* The allowed part sizes */
    static const idx_t parts[4] = {2, 5, 9, 10};

    /* Unbounded knapsack: for each coin, sweep the table ascending */
    for (idx_t p_index = 0; p_index < 4; ++p_index) {
        idx_t p = parts[p_index];
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
