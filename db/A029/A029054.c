/*
 * A029054  Expansion of 1/((1-x)*(1-x^3)*(1-x^8)*(1-x^9)).
 * https://oeis.org/A029054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 1, 3, 8, and 9
 * Range:     u8 n <= 58, u16 n <= 429, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses dynamic programming and a static cache. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for partial results */
    static seq_t *dp = NULL;
    static size_t cap = 0;

    /* Ensure the cache is large enough for indices 0..n */
    if (n + 1 > cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        dp = tmp;
        cap = new_cap;
    }

    /* initialize dp[0] and zero out the rest up to n */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        dp[i] = 0;
    }

    /* parts allowed in the partitions */
    const idx_t parts[4] = {1, 3, 8, 9};

    /* DP: for each part, add contributions for all indices >= part */
    for (int p = 0; p < 4; ++p) {
        idx_t part = parts[p];
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    return dp[n];
}
