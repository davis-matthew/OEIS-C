/*
 * A029433  Expansion of 1/((1-x^7)(1-x^8)(1-x^9)(1-x^11)).
 * https://oeis.org/A029433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in 1/((1-x^7)(1-x^8)(1-x^9)(1-x^11))
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) counts non‑negative integer solutions of 7a+8b+9c+11d = n */
#include <stdlib.h>

seq_t a(idx_t n) {
    size_t N = (size_t)n;                  /* array size */
    seq_t *dp = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (!dp) return 0;                     /* allocation failed */

    for (size_t i = 0; i <= N; ++i) dp[i] = 0;
    dp[0] = 1;                             /* empty combination */

    const idx_t coins[4] = {7, 8, 9, 11};

    for (int ci = 0; ci < 4; ++ci) {
        idx_t coin = coins[ci];
        if (coin > n) continue;
        size_t csz = (size_t)coin;
        for (size_t i = csz; i <= N; ++i) {
            dp[i] = dp[i] + dp[i - csz];
        }
    }

    seq_t result = dp[N];
    free(dp);
    return result;
}
