/*
 * A179052  Range and record values of number of partitions of n into powers
 *          of 10 (cf. A179051).
 * https://oeis.org/A179052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A179051(10*n) – number of partitions of 10*n into powers
 *            of 10 (parts 1,10,100,1000,…)
 * Range:     u8 n <= 65, u16 n <= 677, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    idx_t m = n * 10ULL;                      /* m = 10*n */

    /* Allocate DP array for counting partitions up to m. */
    seq_t *dp = (seq_t *)calloc((size_t)(m + 1), sizeof(seq_t));
    if (!dp)
        return (seq_t)0;                      /* allocation failure – unlikely for n ≤ 10000 */

    dp[0] = (seq_t)1;                         /* one way to make 0 */

    /* For each power of 10 coin, update the DP table. */
    for (idx_t coin = 1; coin <= m; )
    {
        for (idx_t j = coin; j <= m; ++j)
            dp[j] += dp[j - coin];

        /* Prevent overflow of coin*10 after reaching the largest needed coin. */
        if (coin > m / 10)
            break;
        coin *= 10;
    }

    seq_t result = dp[m];
    free(dp);
    return result;
}
