/*
 * A029139  Expansion of 1/((1-x^2)(1-x^3)(1-x^4)(1-x^9)).
 * https://oeis.org/A029139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 2, 3, 4, and 9
 * Range:     u8 n <= 59, u16 n <= 430, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* The function returns the number of ways to write n as a sum of
   parts chosen from {2,3,4,9}.  It works for any implementation
   of seq_t (built-in or arbitrary‑precision). */
seq_t a(idx_t n)
{
    /* Coin denominations */
    static const idx_t coins[4] = {2, 3, 4, 9};

#ifdef __cplusplus
    /* C++: use vector to automatically construct seq_t objects */
    std::vector<seq_t> dp(n + 1, 0);
    dp[0] = 1;
    for (int k = 0; k < 4; ++k) {
        idx_t c = coins[k];
        for (idx_t i = c; i <= n; ++i) {
            dp[i] += dp[i - c];
        }
    }
    return dp[n];
#else
    /* C: use calloc to zero-initialize the array */
    seq_t *dp = calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) return 0;          /* fallback on allocation failure */
    dp[0] = 1;
    for (int k = 0; k < 4; ++k) {
        idx_t c = coins[k];
        for (idx_t i = c; i <= n; ++i) {
            dp[i] += dp[i - c];
        }
    }
    seq_t result = dp[n];
    free(dp);
    return result;
#endif
}
