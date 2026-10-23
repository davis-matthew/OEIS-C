/*
 * A168656  Number of partitions of n such that the smallest part is divisible
 *          by the number of parts.
 * https://oeis.org/A168656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k>=1} sum_{t>=1} P_{\le k-1}(n - k^2*t) where P_{\le
 *            k-1}(m) is the number of partitions of m into at most k-1 parts
 *            (equivalently, with parts ≤ k-1)
 * Range:     u8 n <= 43, u16 n <= 110, u32 n <= 319, u64 n <= 1030,
 *            u128 n <= 3602, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Number of partitions of m with parts only from 1..k-1.
   The algorithm builds the table incrementally: for each k we add
   part size k to the existing partitions.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1 */

    /* Allocate DP array: dp[m] = partitions of m using parts ≤ current_k */
    size_t sz = (size_t)(n + 1);
    seq_t *dp  = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp) return 0;               /* allocation failure → 0 */
    dp[0] = 1;                       /* empty partition */

    seq_t result = 0;
    /* Iterate over the number of parts k; only k with k^2 ≤ n contribute.  */
    for (idx_t k = 1; k * k <= n; ++k) {
        idx_t step = k * k;           /* k^2 */
        /* Add contributions for all t ≥ 1 with k^2 * t ≤ n.  */
        for (idx_t m = step; m <= n; m += step) {
            idx_t rem = n - m;        /* m = k^2 * t */
            result += dp[rem];
        }
        /* Update dp to allow parts up to k (for next k+1).  */
        for (idx_t m = k; m <= n; ++m) {
            dp[m] += dp[m - k];
        }
    }
    free(dp);
    return result;
}
