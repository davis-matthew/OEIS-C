/*
 * A240180  Number of partitions of n such that (least part) = (multiplicity
 *          of greatest part).
 * https://oeis.org/A240180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n such that least part equals
 *            multiplicity of greatest part. Computation is performed by
 *            iterating over all possible multiplicities m and greatest parts
 *            L, counting the partitions of the remaining sum with parts in
 *            [m, L‑1]
 * Range:     u8 n <= 18, u16 n <= 45, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* a(0) = 0 */

    seq_t ans = 0;

    /* integer square root of n (floor) */
    idx_t max_m = 0;
    while ((max_m + 1) * (max_m + 1) <= n)
        ++max_m;

    /* buffer for partition DP */
    size_t N = (size_t)n + 1;
    seq_t *dp = NULL;                  /* allocated per m */

    for (idx_t m = 1; m <= max_m; ++m) {
        /* handle the special case L = m (square partitions) */
        if (n == m * m)
            ans = ans + 1;

        /* L must satisfy n >= m*L, and L > m for the DP part */
        idx_t maxL = n / m;
        if (maxL <= m)
            continue;                  /* no L > m possible */

        idx_t maxHigh = maxL - 1;      /* high value: L-1 */

        /* allocate and zero DP array */
        dp = (seq_t*)calloc(N, sizeof(seq_t));
        if (!dp) return ans;          /* memory failure, return partial result */
        dp[0] = 1;

        /* incremental DP over allowed part sizes h = m .. high */
        for (idx_t h = m; h <= maxHigh; ++h) {
            /* add part size h */
            for (size_t s = h; s < N; ++s)
                dp[s] = dp[s] + dp[s - h];

            idx_t L     = h + 1;
            idx_t prod  = m * (L + 1);
            if (prod > n)
                continue;              /* remaining sum would be negative */

            idx_t Rprime = n - prod;
            ans = ans + dp[Rprime];
        }

        free(dp);
    }

    return ans;
}
