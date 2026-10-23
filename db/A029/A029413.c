/*
 * A029413  Expansion of 1/((1-x^6)*(1-x^7)*(1-x^8)*(1-x^9)).
 * https://oeis.org/A029413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in 1/((1-x^6)(1-x^7)(1-x^8)(1-x^9))
 *            Equivalent to the number of solutions of 6a+7b+8c+9d = n with
 *            a,b,c,d ≥ 0
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    const idx_t coins[4] = {6, 7, 8, 9};

#ifdef __cplusplus
    seq_t *dp = new seq_t[n + 1];            /* value‑initialised in C++ */
#else
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t)); /* zero‑initialised in C */
#endif
    if (!dp) return 0;                      /* allocation failure handled safely */

    dp[0] = (seq_t)1;

    for (int k = 0; k < 4; ++k) {
        idx_t c = coins[k];
        for (idx_t i = c; i <= n; ++i) {
            dp[i] += dp[i - c];
        }
    }

    seq_t result = dp[n];

#ifdef __cplusplus
    delete [] dp;
#else
    free(dp);
#endif
    return result;
}
