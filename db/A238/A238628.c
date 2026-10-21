/*
 * A238628  Number of partitions p of n such that n - max(p) is a part of p.
 * https://oeis.org/A238628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊(n‑1)/2⌋ + (n even ? [#partitions of n with largest part
 *            = n/2] : 0) where #partitions of n with largest part = m is
 *            (p(n,m) - p(n,m‑1)), and p(n,k) is the number of partitions of n
 *            with all parts ≤ k
 * Range:     u8 n <= 33, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* first term: floor((n-1)/2) */
    seq_t term = (seq_t)((n - 1) / 2);

    if (n % 2 == 0)   /* need the extra term for even n */
    {
        idx_t m = n / 2;
        /* dp[j] will hold p(j, current_max) */
        seq_t *dp = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        if (!dp)
            return term;     /* fallback on OOM */

        for (idx_t i = 0; i <= n; ++i)
            dp[i] = 0;
        dp[0] = 1;

        /* partitions with parts up to m-1 (p(n, m-1)) */
        for (idx_t i = 1; i < m; ++i)
        {
            for (idx_t j = i; j <= n; ++j)
                dp[j] += dp[j - i];
        }
        seq_t p_nm1 = dp[n];

        /* add part m to get p(n, m) */
        for (idx_t j = m; j <= n; ++j)
            dp[j] += dp[j - m];
        seq_t p_nm = dp[n];

        free(dp);

        term += (p_nm - p_nm1);
    }

    return term;
}
