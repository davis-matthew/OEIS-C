/*
 * A182086  Number of ways of making change for n Pfennig using Deutschmark
 *          coins.
 * https://oeis.org/A182086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to partition n into
 *            1,2,5,10,50,100,200,500 * (standard coin‑change DP)
 * Range:     u8 n <= 44, u16 n <= 252, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* DP array: number of ways to obtain each value up to n. */
#ifdef __cplusplus
    std::vector<seq_t> dp(n + 1, 0);
    dp[0] = 1;
#else
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp)          /* allocation failure – unlikely but safe */
        return 0;
    dp[0] = 1;
#endif

    /* Coin values available in the Deutsche Mark system. */
    static const idx_t coins[] = {1, 2, 5, 10, 50, 100, 200, 500};
    static const size_t coin_count = sizeof(coins) / sizeof(coins[0]);

    for (size_t c = 0; c < coin_count; ++c) {
        idx_t coin = coins[c];
        for (idx_t i = coin; i <= n; ++i) {
#ifdef __cplusplus
            dp[i] += dp[i - coin];
#else
            dp[i] = dp[i] + dp[i - coin];
#endif
        }
    }

#ifdef __cplusplus
    seq_t result = dp[n];
#else
    seq_t result = dp[n];
    free(dp);
#endif
    return result;
}
