/*
 * A029112  Expansion of 1/((1-x)(1-x^6)(1-x^8)(1-x^11)).
 * https://oeis.org/A029112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in 1/((1-x)(1-x^6)(1-x^8)(1-x^11)),
 *            i.e. the number of ways to write n as a sum of non‑negative
 *            multiples of 1, 6, 8 and 11
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate and initialise array of size n+1 */
    seq_t *dp = (seq_t *)calloc(n + 1, sizeof(seq_t));
    if (!dp)
        return 0;                 /* allocation failure – return 0 */

    dp[0] = 1;                    /* only way to make 0 */

    const idx_t coins[4] = {1, 6, 8, 11};

    for (int ci = 0; ci < 4; ++ci) {
        idx_t d = coins[ci];
        for (idx_t i = d; i <= n; ++i)
            dp[i] += dp[i - d];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
