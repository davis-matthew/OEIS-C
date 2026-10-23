/*
 * A029325  Expansion of 1/((1-x^3)*(1-x^10)*(1-x^11)*(1-x^12)).
 * https://oeis.org/A029325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3, 10, 11, 12
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Count partitions of n using parts 3, 10, 11, 12 */
seq_t a(idx_t n)
{
    /* static cache between calls to avoid repeated allocations */
    static seq_t *dp = NULL;      /* DP array for counts */
    static idx_t allocated = 0;   /* current allocated size */

    /* Ensure the array is large enough for n */
    if (n >= allocated) {
        idx_t new_size = n + 1;                     /* need indices up to n */
        seq_t *new_dp = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_dp) return (seq_t)0;               /* out‑of‑memory, return 0 */

        /* Replace old array */
        free(dp);
        dp = new_dp;
        allocated = new_size;
    }

    /* Compute DP from scratch up to n */
    dp[0] = (seq_t)1;           /* exactly one way to sum to 0 */
    for (idx_t i = 1; i <= n; ++i) dp[i] = (seq_t)0;

    /* Coin values allowed */
    const idx_t coins[4] = {3, 10, 11, 12};

    for (int c = 0; c < 4; ++c) {
        idx_t coin = coins[c];
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    return dp[n];
}
