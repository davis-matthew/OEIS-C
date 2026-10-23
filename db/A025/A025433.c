/*
 * A025433  Number of partitions of n into 9 nonzero squares.
 * https://oeis.org/A025433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 9 nonzero squares
 * Range:     u8 n <= 262, u16 n <= 1912, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    const idx_t K = 9;                     /* exactly 9 squares */

    /* maximum square value that can appear in a partition of n */
    idx_t max_sq = 0;
    while ((max_sq + 1) * (max_sq + 1) <= n)
        ++max_sq;

    const idx_t stride = n + 1;            /* columns per row */

    /* Allocate the DP table: (K+1) rows × (n+1) columns */
    seq_t *dp;
#ifdef __cplusplus
    dp = new seq_t[(K + 1) * stride];
#else
    dp = (seq_t *)calloc((K + 1) * stride, sizeof(seq_t));
#endif
    if (!dp)                 /* allocation failure protection */
        return (seq_t)0;

    dp[0 * stride + 0] = (seq_t)1;           /* empty sum with 0 numbers */

    /* Main DP: add squares one by one */
    for (idx_t t = 1; t <= max_sq; ++t) {
        idx_t val = t * t;                   /* the square value */

        for (idx_t k = 1; k <= K; ++k) {     /* number of parts used */
            for (idx_t s = val; s <= n; ++s) {
                dp[k * stride + s] +=
                    dp[(k - 1) * stride + (s - val)];
            }
        }
    }

    seq_t result = dp[K * stride + n];

#ifdef __cplusplus
    delete [] dp;
#else
    free(dp);
#endif
    return result;
}
