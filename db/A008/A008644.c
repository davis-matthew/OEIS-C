/*
 * A008644  Molien series of 5 X 5 upper triangular matrices over GF( 2 ).
 * https://oeis.org/A008644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 1, 2, 4, 8, and 16
 *            (i.e. the coefficient of x^n in
 *            1/((1-x)(1-x^2)(1-x^4)(1-x^8)(1-x^16))). This is computed by a
 *            simple dynamic‑programming coin‑change routine
 * Range:     u8 n <= 35, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate array for DP, size n+1 (<= 10001 for typical tests) */
    size_t sz = (size_t)(n + 1);
    seq_t *dp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (dp == NULL) return 0; /* memory allocation failure */

    /* initialise */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    /* denominations */
    const idx_t coins[5] = {1, 2, 4, 8, 16};

    /* standard coin‑change DP: for each coin, add ways to make sums */
    for (int c = 0; c < 5; ++c) {
        idx_t coin = coins[c];
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    seq_t ans = dp[n];
    free(dp);
    return ans;
}
