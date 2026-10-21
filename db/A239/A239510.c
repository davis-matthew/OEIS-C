/*
 * A239510  Number of partitions p of n such that if h = min(p), then h is an
 *          (h,0)-separator of p; see Comments.
 * https://oeis.org/A239510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k≥1} Σ_{h≥1} p_{≤k+1}( n – h·(2k+1) – (k+1) ), where
 *            p_{≤t}(m) is the number of partitions of m whose largest part
 *            does not exceed t (equivalently partitions of m into at most t
 *            parts)
 * Range:     u8 n <= 25, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    int N = (int)n;                /* n is small enough for int arithmetic */
    seq_t *dp = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!dp) return 0;            /* allocation failed, return zero */

    /* initialise dp array to zeros */
    for (int i = 0; i <= N; ++i)
        dp[i] = 0;
    dp[0] = 1;                    /* one way to partition 0 */

    /* process part size 1 first (partitions with parts ≤1) */
    for (int i = 1; i <= N; ++i)
        dp[i] += dp[i - 1];

    seq_t ans = 0;

    int kmax = (N - 2) / 3;        /* maximum k such that h≥1 is possible */
    for (int k = 1; k <= kmax; ++k)
    {
        int t = k + 1;             /* current maximal part size */

        /* add contributions of part size t to dp (partitions with parts ≤t) */
        for (int i = t; i <= N; ++i)
            dp[i] += dp[i - t];

        /* for this k, h must satisfy N – h*(2k+1) – t ≥ 0 */
        int hmax = (N - t) / (2 * k + 1);
        if (hmax < 1) continue;

        for (int h = 1; h <= hmax; ++h)
        {
            int R = N - h * (2 * k + 1) - t;   /* remaining sum after fixing k,h */
            ans += dp[R];
        }
    }

    free(dp);
    return ans;
}
