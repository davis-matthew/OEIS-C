/*
 * A079559  Number of partitions of n into distinct parts of the form 2^j-1,
 *          j=1,2,....
 * https://oeis.org/A079559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = number of partitions of n into distinct parts of the form
 *            2^j-1; computed by a classic 0/1 knapsack DP over the Mersenne
 *            numbers
 * Range:     u8 n <= 191, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Build the list of Mersenne numbers <= n */
    seq_t mersenne[64];
    size_t count = 0;
    for (idx_t j = 1; ; ++j) {
        seq_t val = ((seq_t)1 << j) - 1;      /* 2^j - 1 */
        if (val > n) break;
        mersenne[count++] = val;
    }

    /* Allocate DP array of size n+1 */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0; /* allocation failure – should not happen for n<=~10000 */

    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    /* 0/1 knapsack: each Mersenne number can be used at most once */
    for (size_t mi = 0; mi < count; ++mi) {
        seq_t mval = mersenne[mi];
        for (idx_t rest = n; rest >= (idx_t)mval; --rest) {
            dp[rest] += dp[rest - (idx_t)mval];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
