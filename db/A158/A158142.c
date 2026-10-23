/*
 * A158142  Number of nondecreasing integer sequences of length 8 with sum
 *          zero and sum of absolute values 2n.
 * https://oeis.org/A158142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{u=1}^{8} Σ_{v=1}^{8-u} p_u(n) * p_v(n) where p_k(n) is
 *            the number of partitions of n into exactly k positive parts.
 *            This counts non‑decreasing integer sequences of length 8 with
 *            sum 0 and sum of absolute values 2n
 * Range:     u8 n <= 7, u16 n <= 24, u32 n <= 178, u64 n <= 689,
 *            u128 n <= 689, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset starts at 1 */

    /* number of rows (k) = 9 to include p_0 .. p_8,
       number of columns = n+1 for values 0..n */
    const size_t k = 9;
    const size_t stride = (size_t)n + 1;
    const size_t tot_sz = k * stride;
    seq_t *dp = (seq_t *)malloc(tot_sz * sizeof(seq_t));
    if (!dp) return 0;                  /* allocation failure */

    memset(dp, 0, tot_sz * sizeof(seq_t));
    dp[0] = 1;                           /* p_0(0) = 1 */

    /* Fill DP table for partitions into exactly i parts */
    for (idx_t i = 1; i < 9; ++i) {
        for (idx_t j = 1; j <= n; ++j) {
            seq_t val = 0;
            if (j >= i) {               /* term p_i(j-i) */
                val += dp[(size_t)i * stride + (size_t)(j - i)];
            }
            val += dp[(size_t)(i - 1) * stride + (size_t)(j - 1)];  /* term p_{i-1}(j-1) */
            dp[(size_t)i * stride + (size_t)j] = val;
        }
    }

    /* Accumulate the count a(n) */
    seq_t total = 0;
    for (idx_t u = 1; u <= 8; ++u) {
        if (u > n) continue;            /* p_u(n) = 0 for u > n */
        for (idx_t v = 1; v <= 8 - u; ++v) {
            if (v > n) continue;        /* p_v(n) = 0 for v > n */
            total += dp[(size_t)u * stride + (size_t)n] *
                     dp[(size_t)v * stride + (size_t)n];
        }
    }

    free(dp);
    return total;
}
