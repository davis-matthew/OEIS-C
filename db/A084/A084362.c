/*
 * A084362  a(n) = Sum_{lambda_1 + lambda_2 + ... = n} Sum_{k=1..lambda_1} k *
 *          C(lambda_1-1, k-1), where lambda_1 is the largest part of the
 *          partition of n.
 * https://oeis.org/A084362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{m=1..n} ( (m==1?1 : (m+1)*2^{m-2}) * p(n-m,m) ),
 *            where p(k,m) is the number of partitions of k with parts ≤ m.
 *            The implementation directly computes the contribution of each
 *            largest part m while building the partition numbers with a 1‑D
 *            DP
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   weight_for_m
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/weight_for_m.h"

/* Sequence A084362 */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n<1, return 0 safely */
        return (seq_t)0;

    /* dp[i] will hold the number of partitions of i using parts ≤ current m */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp)                /* allocation failed – return 0 as a safe fallback */
        return (seq_t)0;
    dp[0] = (seq_t)1;       /* one empty partition */

    seq_t result = (seq_t)0;

    for (idx_t m = 1; m <= n; ++m) {
        seq_t w = weight_for_m(m);

        /* Update partition counts to include part m */
        for (idx_t i = m; i <= n; ++i)
            dp[i] += dp[i - m];

        /* Add contribution of partitions whose largest part is m */
        result += w * dp[n - m];
    }

    free(dp);
    return result;
}
