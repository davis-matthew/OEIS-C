/*
 * A036006  Number of partitions of n into parts not of the form 25k, 25k+7 or
 *          25k-7. Also number of partitions with at most 6 parts of size 1
 *          and differences between parts at distance 11 are greater than 1.
 * https://oeis.org/A036006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts not of the form *
 *            25k, 25k+7 or 25k-7. This is computed with the standard
 *            un‑bounded * coin‑change DP. dp[0] = 1 and for each allowed part
 *            size p we add * dp[s - p] to dp[s] for all s >= p. This counts
 *            all unordered * partitions. The implementation runs...
 * Range:     u8 n <= 17, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset starts at 1 */

    size_t sz = (size_t)(n + 1);
    seq_t *dp = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!dp) return 0;                  /* allocation failure */

    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    for (idx_t part = 1; part <= n; ++part) {
        idx_t r = part % 25;
        if (r == 0 || r == 7 || r == 18) continue;   /* forbidden part size */

        for (idx_t sum = part; sum <= n; ++sum) {
            dp[sum] += dp[sum - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
