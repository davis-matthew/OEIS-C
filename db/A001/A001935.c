/*
 * A001935  Number of partitions with no even part repeated; partitions of n
 *          in which no parts are multiples of 4.
 * https://oeis.org/A001935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   G.f.: Product_{k>=1, 4 does not divide k} 1/(1-x^k) (=
 *            P(x^4)/P(x)); computed by coin-change DP over parts not
 *            divisible by 4
 * Range:     u8 n <= 18, u16 n <= 51, u32 n <= 157, u64 n <= 526,
 *            u128 n <= 1878, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *a001935_cache = NULL;
static idx_t a001935_len = 0;

seq_t a(idx_t n) {
    if (n < a001935_len) return a001935_cache[n];

    idx_t m = n + 1;
    if (m < 2 * a001935_len) m = 2 * a001935_len;
    if (m < 64) m = 64;

    seq_t *dp = (seq_t *)calloc((size_t)m, sizeof(seq_t));
    for (idx_t i = 0; i < m; i++) dp[i] = 0;
    dp[0] = 1;

    for (idx_t k = 1; k < m; k++) {
        if (k % 4 == 0) continue;
        for (idx_t i = k; i < m; i++) {
            dp[i] += dp[i - k];
        }
    }

    if (a001935_cache) free(a001935_cache);
    a001935_cache = dp;
    a001935_len = m;
    return a001935_cache[n];
}
