/*
 * A240493  Number of partitions of n such that the multiplicity of the least
 *          part is a part.
 * https://oeis.org/A240493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n in which the multiplicity of
 *            the * least part is itself a part. * Computation uses the
 *            recursive formula: * for each least part j and multiplicity m (1
 *            ≤ j ≤ m ≤ n/j): * if j == m => contribution = P_{j+1}(n - j*m) *
 *            else => contribution = P_{j+1}(n - j*m - m) * w...
 * Range:     u8 n <= 19, u16 n <= 47, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* dp[t] holds P_k(t) where current k starts at n+1 and decreases
     * in the main loop. Initially P_{n+1}(0)=1, others 0. */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;          /* memory error: return 0 */

    for (idx_t i = 0; i <= n; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    seq_t ans = (seq_t)0;

    /* Handle j = n explicitly (needs P_{n+1}) */
    if (n >= 1) {
        idx_t j = n;
        idx_t max_m = n / j;          /* this is 1 for n>0 */
        for (idx_t m = j; m <= max_m; ++m) {
            idx_t prod = j * m;        /* prod <= n */
            if (prod > n) continue;
            seq_t s = (seq_t)n - (seq_t)prod;   /* s = n - j*m (>=0) */
            /* j == m holds here */
            ans += dp[(size_t)s];
        }
    }

    /* Main DP loop: update dp for k from n down to 1. */
    for (idx_t k = n; k >= 1; --k) {
        /* Update dp to P_k(t) from P_{k+1}(t) */
        for (idx_t t = k; t <= n; ++t) {
            dp[t] += dp[t - k];
        }

        idx_t j = k - 1;
        if (j == 0) continue;          /* skip j = 0 */

        idx_t max_m = n / j;            /* floor(n/j) */
        for (idx_t m = j; m <= max_m; ++m) {
            idx_t prod = j * m;          /* prod <= n */
            if (prod > n) continue;
            seq_t s = (seq_t)n - (seq_t)prod;   /* s = n - j*m */

            if (j == m) {
                ans += dp[(size_t)s];          /* P_{j+1}(s) */
            } else {
                if (s >= m) {
                    seq_t s2 = s - (seq_t)m;   /* s2 = n - j*m - m */
                    ans += dp[(size_t)s2];
                }
            }
        }
    }

    free(dp);
    return ans;
}
