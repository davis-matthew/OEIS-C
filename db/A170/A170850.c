/*
 * A170850  G.f.: Product_{k>=0} (1 + 3x^(2^k-1) + 3x^(2^k)).
 * https://oeis.org/A170850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A170850: coefficients of the generating function
   Product_{k>=0} (1 + 3 x^(2^k-1) + 3 x^(2^k))
   computed by dynamic programming on the exponents. */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* allocate current DP array */
#ifdef __cplusplus
    seq_t *dp = new seq_t[n + 1];
#else
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
#endif
    /* initialise */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* iterate over k until both 2^k-1 and 2^k exceed n */
    for (idx_t k = 0;; ++k) {
        idx_t two = ((idx_t)1 << k);
        if (two - 1 > n && two > n)
            break;

        idx_t a0 = two - 1;   /* exponent 2^k-1 */
        idx_t a1 = two;       /* exponent 2^k */

        /* allocate new DP array */
#ifdef __cplusplus
        seq_t *newdp = new seq_t[n + 1];
#else
        seq_t *newdp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
#endif
        for (idx_t i = 0; i <= n; ++i)
            newdp[i] = dp[i];

        seq_t mult3 = (seq_t)3;

        if (a0 <= n) {
            for (idx_t i = a0; i <= n; ++i)
                newdp[i] += dp[i - a0] * mult3;
        }
        if (a1 <= n) {
            for (idx_t i = a1; i <= n; ++i)
                newdp[i] += dp[i - a1] * mult3;
        }

        /* free old array */
#ifdef __cplusplus
        delete[] dp;
#else
        free(dp);
#endif
        dp = newdp;
    }

    seq_t result = dp[n];
#ifdef __cplusplus
    delete[] dp;
#else
    free(dp);
#endif
    return result;
}
