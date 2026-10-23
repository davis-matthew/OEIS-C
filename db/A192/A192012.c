/*
 * A192012  Number of ways to use the elements of set {1,..,k}, 0<=k<=3*n,
 *          once each to form a collection of n (possibly empty) sets, each
 *          with at most 3 elements.
 * https://oeis.org/A192012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..3*n} Sum_{i=0..n} Sum_{j=0..3*i-k} k! *C(i,j)
 *            *C(j,k+2*j-3*i) / (i! * 3^(i-j) * 2^(k+j-2*i) )
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* special case n = 0 */
    if (n == 0)
        return (seq_t)1;  /* a(0) = 1 */

    /* total number of elements we may need to partition */
    idx_t max_k = 3 * n;

    /* allocate 4 rows of dp arrays: dp[0..3][0..n] */
    size_t sz = (size_t)(n + 1);
    seq_t *dp[4];
    for (int t = 0; t < 4; ++t)
    {
        dp[t] = (seq_t *)calloc(sz, sizeof(seq_t));
        if (!dp[t])  /* out of memory – fall back */
            return (seq_t)0;
    }

    /* dp[0][0] = 1  for k = 0, b = 0 */
    dp[0][0] = (seq_t)1;
    seq_t total = dp[0][0];   /* includes a(0) contribution */

    for (idx_t k = 1; k <= max_k; ++k)
    {
        int cur  = (int)(k % 4);
        int p1   = (int)((k - 1) % 4);
        int p2   = (int)((k - 2) % 4);
        int p3   = (int)((k - 3) % 4);
        int max_b = (k < n) ? (int)k : (int)n;   /* min(k,n) */

        /* zero the current row entries that we won't fill later */
        dp[cur][0] = (seq_t)0;

        for (int b = 1; b <= max_b; ++b)
        {
            seq_t val = dp[p1][b - 1];             /* block of size 1 */

            if (k >= 2)
            {
                seq_t term2 = (seq_t)(k - 1) * dp[p2][b - 1];  /* block of size 2 */
                val += term2;
            }

            if (k >= 3)
            {
                seq_t term3 = ((seq_t)(k - 1) * (k - 2) / 2) * dp[p3][b - 1]; /* size 3 */
                val += term3;
            }

            dp[cur][b] = val;
            total += val;   /* accumulate into answer */
        }
    }

    /* free temporary arrays */
    for (int t = 0; t < 4; ++t)
        free(dp[t]);

    return total;
}
