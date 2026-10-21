/*
 * A099199  Number of partitions of n into odd triangular numbers.
 * https://oeis.org/A099199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into odd triangular numbers
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    size_t N = (size_t)n;

    /* dp[i] = number of partitions of i using odd triangular numbers */
    seq_t *dp = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!dp)
        return (seq_t)0;

    dp[0] = (seq_t)1;

    for (idx_t k = 1; ; ++k)
    {
        seq_t t = (seq_t)k * (k + 1) / 2;   /* triangular number */

        if (t > n)            /* no further parts needed */
            break;

        /* Keep only odd triangular numbers */
        if ((t & (seq_t)1) == (seq_t)0)
            continue;

        size_t ti = (size_t)t;              /* t fits within N */

        for (size_t i = ti; i <= N; ++i)
            dp[i] = dp[i] + dp[i - ti];
    }

    seq_t result = dp[N];
    free(dp);
    return result;
}
