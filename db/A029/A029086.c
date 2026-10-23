/*
 * A029086  Expansion of 1/((1-x)*(1-x^5)*(1-x^6)*(1-x^8)).
 * https://oeis.org/A029086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 1, 5, 6 and 8
 * Range:     u8 n <= 61, u16 n <= 445, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for the DP array */
    static seq_t *dp = NULL;
    static idx_t  sz = 0;      /* current allocated length (>= n+1) */

    /* allocate / enlarge array if necessary */
    if (sz <= n) {
        idx_t new_sz = n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_sz * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failure sentinel */
        dp = tmp;
        sz = new_sz;
    }

    /* initialise dp[0] and clear the rest up to n */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;

    /* outer‑loop over coins, inner‑loop over sums */
    static const idx_t coins[4] = {1, 5, 6, 8};
    for (int c = 0; c < 4; ++c) {
        idx_t coin = coins[c];
        if (coin > n) continue;
        for (idx_t i = coin; i <= n; ++i)
            dp[i] += dp[i - coin];
    }

    return dp[n];
}
