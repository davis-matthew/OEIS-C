/*
 * A029114  Expansion of 1/((1-x)*(1-x^6)*(1-x^9)*(1-x^11)).
 * https://oeis.org/A029114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n using parts 1, 6, 9, 11
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate an array for dynamic programming */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp)
        abort();   /* allocation failure */

    /* Initialise: only 0 has one way, others none yet */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* Coin denominations */
    static const idx_t coins[4] = {1, 6, 9, 11};

    /* Standard unbounded coin-change DP */
    for (size_t ci = 0; ci < 4; ++ci) {
        idx_t c = coins[ci];
        for (idx_t i = c; i <= n; ++i)
            dp[i] += dp[i - c];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
