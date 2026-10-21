/*
 * A238625  Number of partitions p of n such that 1 + (1/2)*max(p) is a part
 *          of p.
 * https://oeis.org/A238625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{t=1}^{⌊n/2⌋} P(n - 2t - (t+1) if t>1 else 2, 2t)
 *            where P(x, k) is the number of partitions of x with all parts ≤
 *            k. The implementation builds the partition table incrementally
 *            by adding each part size i and, whenever i is even (i = 2t),
 *            adds the corresponding contribution to the answer
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 148, u64 n <= 455,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    size_t N = (size_t)n;
    /* Allocate DP array for partitions with parts up to current i */
    seq_t *dp = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;          /* out of memory – unlikely in tests */

    for (size_t i = 0; i <= N; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;           /* there is one way to partition 0 */

    seq_t ans = (seq_t)0;

    for (idx_t i = 1; i <= n; ++i) {
        size_t ii = (size_t)i;
        /* Update DP to include part size i */
        for (size_t j = ii; j <= N; ++j)
            dp[j] += dp[j - ii];

        /* When i is even, it equals 2t: evaluate its contribution */
        if ((i & (idx_t)1) == 0) {
            idx_t t = i >> 1;
            seq_t add = (seq_t)0;

            if (t == 1) {                  /* special case: t = 1 */
                if (n >= 2)
                    add = dp[(size_t)(n - 2)];
            } else {
                /* need n' = n - 3t - 1 */
                idx_t need = 3 * t + 1;
                if (n >= need)
                    add = dp[(size_t)(n - need)];
            }
            ans += add;
        }
    }

    free(dp);
    return ans;
}
