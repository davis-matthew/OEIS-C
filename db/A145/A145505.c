/*
 * A145505  a(n+1) = a(n)^2 + 2*a(n) - 2 and a(1) = 5.
 * https://oeis.org/A145505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = a(n)^2 + 2*a(n) - 2, a(1) = 5
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;    /* cache[0] unused, cache[1] = a(1) */
static idx_t cache_len = 0;    /* largest index initialise in 'cache' */

/* Ensure that cache contains values up to at least 'n' (n ≥ 1).  */
static void
ensure_cache(idx_t n)
{
    if (n < 1)
        return;

    if (cache_len >= n)
        return;

    /* Allocate or reallocate space for indices 0..n (index 0 is unused). */
    size_t new_len = (size_t)n + 1;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!cache)
        abort();  /* allocation failure */

    /* Initialise from scratch if cache is empty. */
    if (cache_len == 0) {
        cache[1] = (seq_t)5;      /* a(1) */
        cache_len = 1;
    }

    /* Compute missing terms. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        cache[i] = prev * prev + (seq_t)2 * prev - (seq_t)2;
    }
    cache_len = n;
}

/* Public API: returns a(n) for n ≥ 1. */
seq_t
a(idx_t n)
{
    if (n < 1)          /* behavior below the offset is irrelevant */
        return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
