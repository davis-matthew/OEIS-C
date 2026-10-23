/*
 * A045849  Number of nonnegative solutions of x1^2 + x2^2 + ... + x7^2 = n.
 * https://oeis.org/A045849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in (1 + q + q^4 + q^9 + ...)^7
 * Range:     u8 n <= 9, u16 n <= 150, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    /* For n = 0 the only solution is all zeros */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Pre‑compute the squares up to floor(sqrt(n)) */
    idx_t K = _floor_isqrt_u64(n);
    idx_t *sqr = NULL;
    if (K > 0) {
        sqr = (idx_t *)calloc((size_t)(K + 1), sizeof(idx_t));
        for (idx_t i = 0; i <= K; ++i)
            sqr[i] = i * i;
    }

    /* Dynamic programming arrays */
    size_t sz = (size_t)(n + 1);
    seq_t *dp[2];
    dp[0] = (seq_t *)calloc(sz, sizeof(seq_t));
    dp[1] = (seq_t *)calloc(sz, sizeof(seq_t));
    dp[0][0] = (seq_t)1;   /* base: zero variables sum to zero */

    idx_t cur = 0, nxt = 1;
    for (idx_t var = 0; var < 7; ++var) {     /* add one variable each loop */
        /* clear next array */
        for (size_t i = 0; i < sz; ++i)
            dp[nxt][i] = (seq_t)0;

        for (size_t sum = 0; sum < sz; ++sum) {
            seq_t val = dp[cur][sum];
            if (val == (seq_t)0) continue;

            /* try all possible squares for the current variable */
            for (idx_t j = 0; j <= K; ++j) {
                idx_t t = sum + sqr[j];
                if (t > n) break;
                dp[nxt][t] += val;
            }
        }
        /* swap */
        idx_t tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    seq_t result = dp[cur][n];

    /* free memory */
    free(dp[0]);
    free(dp[1]);
    free(sqr);

    return result;
}
