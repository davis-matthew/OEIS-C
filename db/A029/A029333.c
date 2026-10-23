/*
 * A029333  Expansion of 1/((1-x^4)*(1-x^5)*(1-x^7)*(1-x^9)).
 * https://oeis.org/A029333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the number of partitions of n into parts 4, 5, 7, and 9
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static buffer reused across calls, allocated lazily */
static seq_t *dp = NULL;
static size_t dp_cap = 0;

/* Return the n‑th term of OEIS A029333 */
seq_t a(idx_t n)
{
    /* Ensure the buffer is large enough for indices 0..n */
    size_t required = (size_t)n + 1;
    if (dp_cap < required) {
        seq_t *tmp = (seq_t *)realloc(dp, required * sizeof(seq_t));
        if (!tmp)
            abort();                    /* allocation failed – terminate */
        dp = tmp;
        dp_cap = required;
    }

    /* Initialise dp[0..n] */
    for (size_t i = 0; i < required; ++i)
        dp[i] = 0;
    dp[0] = 1;                         /* empty partition */

    /* Dynamic programming over allowed part sizes */
    const idx_t parts[4] = {4, 5, 7, 9};
    for (int p = 0; p < 4; ++p) {
        idx_t coin = parts[p];
        if (coin > n)
            continue;                  /* skip if part exceeds n */
        for (idx_t i = coin; i <= n; ++i)
            dp[(size_t)i] += dp[(size_t)(i - coin)];
    }

    return dp[(size_t)n];
}
