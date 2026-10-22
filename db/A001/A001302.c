/*
 * A001302  Number of ways of making change for n cents using coins of 1, 2,
 *          5, 10, 25, 50 cents.
 * https://oeis.org/A001302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: 1/((1-x)*(1-x^2)*(1-x^5)*(1-x^10)*(1-x^25)*(1-x^50));
 *            computed by the coin-change DP dp[i] += dp[i-c] for c in
 *            {1,2,5,10,25,50}, dp[0] = 1
 * Range:     u8 n <= 42, u16 n <= 205, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static const int coins[] = {1, 2, 5, 10, 25, 50};
    int c;
    idx_t i;
    seq_t result;
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) return 0;

    dp[0] = 1;
    for (i = 1; i <= n; i++) dp[i] = 0;

    for (c = 0; c < 6; c++) {
        idx_t coin = (idx_t)coins[c];
        for (i = coin; i <= n; i++) {
            dp[i] += dp[i - coin];
        }
    }

    result = dp[n];
    free(dp);
    return result;
}
