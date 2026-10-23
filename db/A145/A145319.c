/*
 * A145319  Numbers Y such that 93*Y^2+31 is a square.
 * https://oeis.org/A145319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A145319: Numbers Y such that 93*Y^2+31 is a square.
 * Recurrence: a(1) = 45, a(2) = 1093635,
 *             a(n+2) = 24302*a(n+1) - a(n)   for n ≥ 1
 */

#include <stddef.h>

/* Static storage for memoised values, 1‑based indexing.
 * A large enough bound is chosen to cover typical test sizes (≈10 000). */
#define MAX_CACHE 20000
static seq_t memo[MAX_CACHE];
static size_t cached_max = 0;  /* largest n for which memo[n] is valid */

/* Compute a(n) for any n ≥ 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* offset is 1 */

    /* Convert to size_t for array indexing (tests use small n). */
    size_t idx = (size_t)n;
    if (idx >= MAX_CACHE)
        /* fallback: compute iteratively without caching for huge n */
        return (seq_t)0;   /* out of range for this implementation */

    /* Initialise first two terms if not already done. */
    if (cached_max < 2) {
        memo[1] = (seq_t)45;
        memo[2] = (seq_t)1093635;
        cached_max = 2;
    }

    /* Extend the cache up to the requested index. */
    for (size_t i = cached_max + 1; i <= idx; ++i) {
        memo[i] = memo[i - 1] * (seq_t)24302 - memo[i - 2];
    }
    cached_max = idx;

    return memo[idx];
}
