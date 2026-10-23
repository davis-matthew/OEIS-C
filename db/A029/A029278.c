/*
 * A029278  Expansion of 1/((1-x^3)*(1-x^5)*(1-x^7)*(1-x^8)).
 * https://oeis.org/A029278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3, 5, 7 and 8.
 *            Compute by the standard coin‑change DP with combinations counted
 *            once
 * Range:     u8 n <= 97, u16 n <= 679, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Coins allowed in the partitions */
    static const idx_t coin[4] = { 3, 5, 7, 8 };
    const idx_t coins = 4;

#   ifdef __cplusplus
    /* C++ implementation – use a local vector so each call starts fresh. */
    std::vector<seq_t> dp(n + 1, (seq_t)0);
    dp[0] = (seq_t)1;
    for (idx_t c = 0; c < coins; ++c)
        for (idx_t i = coin[c]; i <= n; ++i)
            dp[i] += dp[i - coin[c]];
    return dp[n];
#   else
    /* C implementation – allocate a fresh array each call. */
    seq_t *dp = calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp) abort();                /* allocation failure */
    dp[0] = (seq_t)1;
    for (idx_t c = 0; c < coins; ++c)
        for (idx_t i = coin[c]; i <= n; ++i)
            dp[i] += dp[i - coin[c]];
    seq_t res = dp[n];
    free(dp);
    return res;
#   endif
}
