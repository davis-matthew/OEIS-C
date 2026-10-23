/*
 * A029113  Expansion of 1/((1-x)*(1-x^6)*(1-x^9)*(1-x^10)).
 * https://oeis.org/A029113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 1,6,9,10
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: dynamic programming recomputed each call. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache reused for all calls */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;

    /* coins allowed in the partitions */
    static const idx_t coins[4] = {1, 6, 9, 10};

    /* make sure the array is large enough for index n */
    if (dp_size <= n) {
        idx_t new_size = n + 1;
        seq_t *new_dp = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_dp) {
            /* allocation failure – return 0 as a safe fallback */
            return (seq_t)0;
        }
        if (dp)
            free(dp);
        dp = new_dp;
        dp_size = new_size;
    }

    /* clear entries up to n */
    dp[0] = (seq_t)1;                /* one way to make 0: choose nothing */
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* dynamic programming: for each coin, add its contributions */
    for (int c = 0; c < 4; ++c) {
        idx_t coin = coins[c];
        for (idx_t i = coin; i <= n; ++i)
            dp[i] += dp[i - coin];
    }

    return dp[n];
}
