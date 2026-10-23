/*
 * A035380  Number of partitions of n into parts 6k+1 or 6k+2.
 * https://oeis.org/A035380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts congruent to 1 or 2
 *            modulo 6
 * Range:     u8 n <= 33, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* sequence offset starts at 1 */

    /* allocate DP array of size n+1 */
    size_t sz = (size_t)(n + 1);   /* safe cast; n <= 10000 in tests */
    seq_t *dp = (seq_t *)malloc(sizeof(seq_t) * sz);
    if (!dp)
        return (seq_t)0;          /* allocation failure – unlikely in tests */

    /* initialize */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* DP over allowed part sizes: 6k+1 or 6k+2 */
    for (idx_t s = 1; s <= n; ++s)
    {
        if ((s % 6 == 1) || (s % 6 == 2))
        {
            for (idx_t i = s; i <= n; ++i)
                dp[i] += dp[i - s];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
