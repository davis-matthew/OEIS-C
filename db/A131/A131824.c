/*
 * A131824  Main diagonal of triangle A131823: a(n) = A131823(n,n) for n>=0.
 * https://oeis.org/A131824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A131824: a(n) = coefficient of x^n in
   Product_{i=0..n-1} (1 + x^(2^i))^(n-i)  (main diagonal of triangle A131823) */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* n = 0 is a special case */
    if (n == 0) return (seq_t)1;

    /* find the largest i such that 2^i <= n */
    idx_t i_max = 0;
    idx_t pow2 = 1;
    while (pow2 <= n) {
        i_max++;
        pow2 <<= 1;
    }
    i_max--;   /* now 2^i_max <= n < 2^(i_max+1) */

    /* allocate DP arrays */
    seq_t *dp  = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *dp2 = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp || !dp2) {
        /* memory failure – return 0 as a sentinel value */
        free(dp);
        free(dp2);
        return 0;
    }

    dp[0] = (seq_t)1;

    for (idx_t i = 0; i <= i_max; ++i) {
        /* clear the new array */
        memset(dp2, 0, (size_t)(n + 1) * sizeof(seq_t));

        idx_t shift_base = (idx_t)1 << i;          /* 2^i */
        idx_t maxk       = n / shift_base;          /* largest k with k*2^i <= n */
        if (maxk > (n - i))
            maxk = n - i;                          /* exponent bound (n-i) */

        seq_t C = (seq_t)1;                        /* binomial C(n-i, k) */

        for (idx_t k = 0; k <= maxk; ++k) {
            idx_t shift = k * shift_base;
            for (idx_t j = 0; j + shift <= n; ++j) {
                dp2[j + shift] += dp[j] * C;
            }
            if (k < maxk) {
                seq_t num = (seq_t)(n - i - k);
                seq_t den = (seq_t)(k + 1);
                C = (C * num) / den;               /* next binomial coefficient */
            }
        }

        /* swap dp and dp2 buffers for next iteration */
        seq_t *tmp = dp;
        dp = dp2;
        dp2 = tmp;
    }

    seq_t result = dp[n];
    free(dp);
    free(dp2);
    return result;
}
