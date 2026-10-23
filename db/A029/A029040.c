/*
 * A029040  Expansion of 1/((1-x)(1-x^3)(1-x^5)(1-x^8)).
 * https://oeis.org/A029040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 1, 3, 5 and 8. This
 *            implementation uses dynamic programming with a single array
 * Range:     u8 n <= 48, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, free */

/* Return the n‑th term of sequence A029040. */
seq_t a(idx_t n)
{
    /* Size of the DP array */
    size_t sz = (size_t)n + 1U;
    seq_t *dp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp) return (seq_t)0;  /* allocation failed */

    /* Initialise dp[0] = 1, others to 0 */
    dp[0] = (seq_t)1;
    for (size_t i = 1; i < sz; ++i) dp[i] = (seq_t)0;

    /* Allowed parts */
    static const idx_t parts[4] = {1, 3, 5, 8};

    /* DP: for each part, add its contribution to all sums i ≥ p. */
    for (int pi = 0; pi < 4; ++pi) {
        idx_t part = parts[pi];
        for (size_t i = (size_t)part; i < sz; ++i) {
            dp[i] += dp[i - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
