/*
 * A003105  Schur's 1926 partition theorem: number of partitions of n into
 *          parts 6n+1 or 6n-1.
 * https://oeis.org/A003105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   G.f.: 1/Product_{k>=0} (1-x^(6*k+1))*(1-x^(6*k+5)); a(n) =
 *            number of partitions of n into parts congruent to 1 or 5 mod 6,
 *            computed by coin-change DP
 * Range:     u8 n <= 44, u16 n <= 118, u32 n <= 358, u64 n <= 1190,
 *            u128 n <= 4237, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    for (idx_t i = 0; i <= n; i++) dp[i] = 0;
    dp[0] = 1;

    for (idx_t part = 1; part <= n; part++) {
        idx_t r = part % 6;
        if (r == 1 || r == 5) {
            for (idx_t i = part; i <= n; i++) {
                dp[i] += dp[i - part];
            }
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
