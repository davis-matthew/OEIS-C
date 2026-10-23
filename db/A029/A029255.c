/*
 * A029255  Expansion of 1/((1-x^3)*(1-x^4)*(1-x^6)*(1-x^11)).
 * https://oeis.org/A029255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into the parts 3, 4, 6 and 11
 * Range:     u8 n <= 95, u16 n <= 665, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *dp  = NULL;
    static idx_t  sz  = 0;   /* current size of the cache (0 … sz-1 are valid) */

    /* If we already have enough values, just return the stored one. */
    if (dp && n < sz)
        return dp[n];

    /* We need to compute up to n (inclusive).  Allocate a new array. */
    idx_t new_sz = n + 1;
    seq_t *new_dp = (seq_t *)calloc(new_sz, sizeof(seq_t));
    if (!new_dp)
        return (seq_t)0;          /* OOM – return 0, the caller will ignore it */

    /* Base case: one way to make 0 (empty partition). */
    new_dp[0] = (seq_t)1;

    /* The available coin denominations. */
    const idx_t coins[4] = {3, 4, 6, 11};

    /* Classic unbounded‑knapsack DP: for each coin add its contributions. */
    for (int c = 0; c < 4; ++c) {
        idx_t coin = coins[c];
        for (idx_t i = coin; i < new_sz; ++i) {
            new_dp[i] += new_dp[i - coin];
        }
    }

    /* Replace the old cache with the new one. */
    free(dp);
    dp = new_dp;
    sz = new_sz;

    return dp[n];
}
