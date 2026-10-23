/*
 * A029204  Expansion of 1/((1-x^2)*(1-x^5)*(1-x^8)*(1-x^9)).
 * https://oeis.org/A029204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the number of partitions of n into parts 2, 5, 8, and 9
 * Range:     u8 n <= 91, u16 n <= 644, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Number of partitions of n into parts 2, 5, 8, and 9 */
seq_t a(idx_t n)
{
    static seq_t *dp = NULL; /* dynamic programming table */
    static idx_t sz = 0;     /* current capacity of dp */

    /* extend the table if necessary */
    if (sz <= n) {
        size_t new_sz = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(dp, new_sz * sizeof(seq_t));
        if (!tmp && new_sz != 0) exit(1);
        /* initialize the newly allocated part to zero */
        for (size_t i = sz; i < new_sz; ++i)
            tmp[i] = 0;
        dp = tmp;
        sz = new_sz;
    }

    /* recompute the table up to n (O(n) work per call) */
    dp[0] = 1;                 /* only the empty partition */
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;             /* clear previous values */

    const idx_t parts[4] = {2, 5, 8, 9};
    for (int pi = 0; pi < 4; ++pi) {
        idx_t p = parts[pi];
        for (idx_t i = p; i <= n; ++i)
            dp[i] = dp[i] + dp[i - p];
    }

    return dp[n];
}
