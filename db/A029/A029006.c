/*
 * A029006  Expansion of 1/((1-x)*(1-x^2)*(1-x^3)*(1-x^12)).
 * https://oeis.org/A029006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of partitions of n into parts 1, 2, 3 and 12 */

#include <stdlib.h>

/* Persistent cache reused for all calls to reduce allocations */
static seq_t *dp   = NULL;  /* dp[i] will hold a(i) for i up to dp_capacity-1 */
static idx_t dp_capacity = 0;  /* current allocated length of dp */

/* Ensure dp is large enough to store values up to index n */
static void ensure_capacity(idx_t n)
{
    if (dp_capacity >= n + 1) return;          /* already big enough */

    seq_t *new_dp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
    if (!new_dp) return;                       /* out of memory – undefined */

    dp = new_dp;
    dp_capacity = n + 1;
}

seq_t a(idx_t n)
{
    /* Make sure we have space for indices 0 … n  */
    ensure_capacity(n);

    /* Initialise dp[0] and zeros up to n  */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;

    /* Multiset of allowed parts */
    static const idx_t coins[] = {1, 2, 3, 12};
    const size_t num_coins = sizeof(coins) / sizeof(coins[0]);

    /* Classic coin‑change DP – order of loops guarantees order‑independent partitions */
    for (size_t c = 0; c < num_coins; ++c) {
        idx_t coin = coins[c];
        if (coin > n) continue;
        for (idx_t i = coin; i <= n; ++i)
            dp[i] += dp[i - coin];
    }

    return dp[n];
}
