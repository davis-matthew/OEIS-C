/*
 * A029282  Expansion of 1/((1-x^3)*(1-x^5)*(1-x^8)*(1-x^9)).
 * https://oeis.org/A029282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3, 5, 8, 9 (OEIS
 *            A029282)
 * Range:     u8 n <= 106, u16 n <= 739, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *dp   = NULL;   /* DP array: dp[i] = # of partitions of i */
    static size_t cap  = 0;      /* allocated capacity (number of entries) */

    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        seq_t *tmp = (seq_t *)realloc(dp, needed * sizeof(seq_t));
        if (!tmp && needed > 0) {
            exit(EXIT_FAILURE);
        }
        dp  = tmp;
        cap = needed;
    }

    /* Re‑compute the DP table from scratch for the current n. */
    memset(dp, 0, cap * sizeof(seq_t));
    dp[0] = 1;                     /* base case: one way to make 0 */

    const idx_t coins[4] = {3, 5, 8, 9};
    for (int c = 0; c < 4; ++c) {
        idx_t coin = coins[c];
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    return dp[n];
}
