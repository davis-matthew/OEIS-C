/*
 * A035434  Number of partitions of n into parts 7k+2 or 7k+6.
 * https://oeis.org/A035434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of partitions of n into parts 7k+2 or 7k+6. Computed by a
 *            standard coin-change dynamic programming: dp[0] = 1, and for
 *            each allowed part p <= n, for i = p .. n do dp[i] += dp[i-p]
 * Range:     u8 n <= 61, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* for safety, although offset is 1 */

    /* allocate DP array */
    seq_t *dp = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return 0;                    /* allocation failure, unlikely */

    /* initialise */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;

    /* iterate over all allowed part sizes */
    for (idx_t p = 2; p <= n; ++p)
    {
        if (p % 7 == 2 || p % 7 == 6)
        {
            for (idx_t i = p; i <= n; ++i)
            {
                dp[i] += dp[i - p];
            }
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
