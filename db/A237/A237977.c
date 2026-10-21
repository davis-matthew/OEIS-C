/*
 * A237977  Number of strict partitions of n such that (least part) <= number
 *          of parts.
 * https://oeis.org/A237977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{k≥1} ( p(n - k(k-1)/2, k) – p(n - k*k - k(k-1)/2, k) )
 *            where p(m, k) = number of partitions of m into exactly k (not
 *            necessarily distinct) parts. The function uses a dynamic
 *            programming table for p(m, k)
 * Range:     u8 n <= 30, u16 n <= 78, u32 n <= 237, u64 n <= 791,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Compute a(n) – OEIS A237977. */
seq_t a(idx_t n)
{
    /* a(0) = 0 by definition of the offset. */
    if (n == 0)
        return 0;

    /* Determine maximum number of parts needed:  k(k+1)/2 ≤ n */
    idx_t kmax = 0;
    while ((kmax + 1) * (kmax + 2) / 2 <= n)
        ++kmax;

    /* Allocate DP table: dp[sum*(kmax+1) + parts] = p(sum, parts) */
    size_t stride = (size_t)(kmax + 1);          /* columns */
    size_t rows   = (size_t)(n + 1);            /* rows: 0…n */
    seq_t *dp = (seq_t *)calloc(rows * stride, sizeof(seq_t));
    if (!dp) /* malloc failed – return 0 as a fallback */
        return 0;

    /* Base case: p(0,0) = 1 */
    dp[0 * stride + 0] = 1;

    /* Fill DP table */
    for (idx_t sum = 1; sum <= n; ++sum)
    {
        for (idx_t parts = 1; parts <= kmax && parts <= sum; ++parts)
        {
            seq_t val = dp[(sum - 1) * stride + (parts - 1)];
            if (sum >= parts)
                val += dp[(sum - parts) * stride + parts];
            dp[sum * stride + parts] = val;
        }
    }

    /* Accumulate the answer using the derived formula */
    seq_t result = 0;
    for (idx_t k = 1; k <= kmax; ++k)
    {
        /* t1 = k*(k-1)/2 */
        idx_t t1 = k * (k - 1) / 2;
        if (n < t1)        /* safety check; should not happen */
            continue;
        idx_t sum1 = n - t1;                 /* n - k(k-1)/2 */
        seq_t val1 = dp[sum1 * stride + k];

        /* t2 = k*k + t1 */
        idx_t t2 = k * k + t1;
        seq_t val2 = 0;
        if (n >= t2)
        {
            idx_t sum2 = n - t2;             /* n - k*k - k(k-1)/2 */
            val2 = dp[sum2 * stride + k];
        }

        result += val1 - val2;               /* val1 ≥ val2 always */
    }

    free(dp);
    return result;
}
