/*
 * A008639  Number of partitions of n into at most 10 parts.
 * https://oeis.org/A008639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in 1 / Π_{k=1}^{10} (1 - x^k),
 *            computed by the standard one‑dimensional partition DP
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 234, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free */

static seq_t *dp = NULL;   /* cache of partition counts */
static size_t cap = 0;     /* allocated size of dp */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache_size(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (cap >= needed) return;
    seq_t *newdp = (seq_t *)malloc(needed * sizeof(seq_t));
    free(dp);
    dp = newdp;
    cap = needed;
}

seq_t a(idx_t n)
{
    /* Make sure the cache is big enough for indices 0…n. */
    ensure_cache_size(n);

    /* Initialize the DP array for this call. */
    dp[0] = (seq_t)1;
    for (size_t i = 1; i <= (size_t)n; ++i) dp[i] = (seq_t)0;

    /* Classic partition recurrence with parts 1…10. */
    for (int k = 1; k <= 10; ++k) {
        for (size_t i = (size_t)k; i <= (size_t)n; ++i) {
            dp[i] += dp[i - (size_t)k];
        }
    }

    return dp[(size_t)n];
}
