/*
 * A210249  Number of partitions of n in which all parts are less than n/2.
 * https://oeis.org/A210249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n in which all parts are less
 *            than n/2 (i.e., partitions with parts <= floor((n-1)/2))
 * Range:     u8 n <= 17, u16 n <= 43, u32 n <= 127, u64 n <= 416,
 *            u128 n <= 1458, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* special case */
    if (n == 0) return 1;

    /* maximum part allowed */
    idx_t limit = (n - 1) / 2;

    /* DP array */
    seq_t *dp;
#ifdef __cplusplus
    /* Use a vector in C++ to ensure proper construction/destruction of seq_t */
    std::vector<seq_t> dp_vec(static_cast<size_t>(n + 1));
    dp = dp_vec.data();
#else
    /* Plain malloc in C */
    dp = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return 0;                 /* allocation failure */
#endif

    /* initialize to zero */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;                         /* only one way to partition 0 */

    /* dynamic programming */
    for (idx_t part = 1; part <= limit; ++part) {
        for (idx_t sum = part; sum <= n; ++sum) {
            dp[sum] += dp[sum - part];
        }
    }

    seq_t result = dp[n];
#ifndef __cplusplus
    free(dp);
#endif
    return result;
}
