/*
 * A001996  Number of partitions of n into parts 2, 3, 4, 5, 6, 7.
 * https://oeis.org/A001996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: 1/((1-x^2)*(1-x^3)*(1-x^4)*(1-x^5)*(1-x^6)*(1-x^7));
 *            computed by coin-change DP over parts 2..7
 * Range:     u8 n <= 30, u16 n <= 118, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n) {
    seq_t *dp;
    seq_t result;
    idx_t i;
    int p;

    dp = (seq_t *)malloc(((size_t)n + 1) * sizeof(seq_t));
    for (i = 0; i <= n; i++) {
        dp[i] = 0;
    }
    dp[0] = 1;

    for (p = 2; p <= 7; p++) {
        for (i = (idx_t)p; i <= n; i++) {
            dp[i] += dp[i - (idx_t)p];
        }
    }

    result = dp[n];
    free(dp);
    return result;
}
