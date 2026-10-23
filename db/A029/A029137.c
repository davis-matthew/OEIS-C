/*
 * A029137  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^4)*(1-x^7)).
 * https://oeis.org/A029137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   number of partitions of n into parts 2, 3, 4, and 7 (order does
 *            not matter)
 * Range:     u8 n <= 55, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Allocate an array large enough to hold all DP states up to n. */
    seq_t *dp = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return 0;  /* Allocation failure – return zero as a fallback. */

    /* Initialise all DP cells to zero and set the base case. */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;

    /* The allowed parts: 2, 3, 4, and 7. */
    static const idx_t parts[4] = {2, 3, 4, 7};

    /* Standard integer–partition DP: for each part, add ways to form each sum. */
    for (int p = 0; p < 4; ++p) {
        idx_t part = parts[p];
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
