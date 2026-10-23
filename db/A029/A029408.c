/*
 * A029408  Expansion of 1/((1-x^5)*(1-x^8)*(1-x^11)*(1-x^12)).
 * https://oeis.org/A029408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into the parts 5, 8, 11, and 12
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of dp values to avoid reallocating on every call */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;   /* current allocated size of dp   */

    /* ensure the array is large enough for n */
    if (n + 1 > dp_size) {
        seq_t *tmp = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        if (tmp == NULL)   /* memory allocation failed */
            return 0;
        /* discard old cache (it will be freed on program exit) */
        if (dp) free(dp);
        dp = tmp;
        dp_size = n + 1;
    }

    /* initialise the DP table for the new range */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;   /* base case: one way to form 0 (use no parts) */

    /* fixed set of allowed parts */
    const idx_t parts[4] = {5, 8, 11, 12};

    /* classic coin‑change DP (order of summands irrelevant) */
    for (int pi = 0; pi < 4; ++pi) {
        idx_t p = parts[pi];
        if (p > n) continue;
        for (idx_t i = p; i <= n; ++i)
            dp[i] = dp[i] + dp[i - p];
    }

    return dp[n];
}
