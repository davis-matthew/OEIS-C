/*
 * A049685  a(n) = L(4*n+2)/3, where L=A000032 (the Lucas sequence).
 * https://oeis.org/A049685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 7*a(n-1) - a(n-2), a(0)=1, a(1)=6
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a memoised cache of already computed terms
 * and fills it up to the required index using the linear recurrence.
 * The cache is expanded with realloc as necessary.  All arithmetic is
 * performed on the type seq_t, which may be 8‑, 16‑, 32‑, 64‑, 128‑bit
 * or arbitrary‑precision. */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[n] holds a(n) */
static idx_t cache_size = 0;  /* number of cache entries currently allocated */

/* Ensure that terms up to index n are present in the cache. */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size) return;          /* nothing to do */

    /* Expand cache to hold indices 0..n */
    size_t new_size = (size_t)(n + 1);
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

    /* Initialise the base terms if they are not already present */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;           /* a(0) */
        if (new_size > 1) cache[1] = (seq_t)6; /* a(1) */
    } else if (cache_size == 1) {
        cache[1] = (seq_t)6;           /* a(1) */
    }

    /* Fill remaining entries using the recurrence a(k) = 7*a(k-1) - a(k-2) */
    for (idx_t i = (cache_size > 1 ? cache_size : 2); i < new_size; ++i) {
        cache[i] = (seq_t)7 * cache[i - 1] - cache[i - 2];
    }

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    /* Special handling for the first two indices (optional but keeps cache
     * minimal if called with small n only). */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    /* Ensure all needed terms are computed and cached. */
    ensure_cache(n);

    return cache[n];
}
