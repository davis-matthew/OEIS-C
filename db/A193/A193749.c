/*
 * A193749  Number of partitions of n into distinct parts that are squares or
 *          triangular numbers (A005214).
 * https://oeis.org/A193749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts that are
 *            squares or triangular numbers
 * Range:     u8 n <= 113, u16 n <= 250, u32 n <= 250, u64 n <= 250,
 *            u128 n <= 250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Allocate an array marking which numbers 0..n are allowed parts. */
    size_t sz = (size_t)n + 1;
    bool *allowed = (bool *)calloc(sz, sizeof(bool));
    if (!allowed) return 0;                     /* unlikely path, defensive */

    /* Mark square numbers up to n. */
    for (idx_t i = 1; ; ++i) {
        seq_t sq = (seq_t)i * i;
        if (sq > n) break;
        allowed[(size_t)sq] = true;
    }

    /* Mark triangular numbers up to n. */
    for (idx_t i = 1; ; ++i) {
        seq_t t = (seq_t)i * (i + 1) / 2;
        if (t > n) break;
        allowed[(size_t)t] = true;
    }

    /* Subset‑sum DP counting partitions into distinct allowed parts. */
    seq_t *dp = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp) { free(allowed); return 0; }       /* defensive */

    dp[0] = 1;
    for (idx_t s = 1; s <= n; ++s) {
        if (!allowed[(size_t)s]) continue;
        for (idx_t j = n; j >= s; --j)
            dp[j] = dp[j] + dp[j - s];
    }

    seq_t result = dp[n];
    free(dp);
    free(allowed);
    return result;
}
