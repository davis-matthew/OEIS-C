/*
 * A025798  Expansion of g.f. 1/((1 - x^2)*(1 - x^3)*(1 - x^9)).
 * https://oeis.org/A025798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 2, 3 and 9 (i.e. the
 *            coefficient of xⁿ in 1/((1‑x²)(1‑x³)(1‑x⁹))), a(0)=1, a(n)=0 for
 *            n<0
 * Range:     u8 n <= 158, u16 n <= 2651, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate array of size n+1 */
    size_t size = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(size * sizeof(seq_t));
    if (!dp) return 0;               /* allocation failure */

    /* initialise: only empty partition for 0 */
    dp[0] = 1;
    for (size_t i = 1; i <= n; ++i) dp[i] = 0;

    /* coin values */
    int coins[3] = {2, 3, 9};

    /* classic unbounded‑knapsack DP for combinations */
    for (int c = 0; c < 3; ++c) {
        size_t coin = coins[c];
        for (size_t i = coin; i <= n; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
