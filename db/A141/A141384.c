/*
 * A141384  Trace of the n-th power of a certain 8X8 adjacency matrix.
 * https://oeis.org/A141384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=8, a(1)=8, a(2)=32, a(3)=158, a(4)=828 For n ≥ 5: a(n) =
 *            8*a(n-1) – 16*a(n-2) + 10*a(n-3) – a(n-4)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Expand the dynamic cache if index n is out of current bounds. */
static void expand_cache(idx_t n, seq_t **cache, size_t *cap)
{
    size_t required = (size_t)n + 1;          /* cache must hold indices 0..n */
    if (*cap < required) {
        size_t new_cap = (*cap) ? (*cap) * 2 : 8;
        while (new_cap < required) new_cap *= 2;
        *cache = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
        if (*cache == NULL) exit(1);          /* allocation failure */
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Persistent cache state */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Ensure we have enough space in the cache. */
    expand_cache(n, &cache, &cache_cap);

    /* Initialise first five terms once. */
    if (cache_len == 0) {
        cache[0] = (seq_t)8;
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)32;
        cache[3] = (seq_t)158;
        cache[4] = (seq_t)828;
        cache_len = 5;
    }

    /* If requested index is within the cached range, return it. */
    if ((size_t)n < cache_len) {
        return cache[(size_t)n];
    }

    /* Compute remaining terms up to n using the recurrence. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        cache[i] =
            (seq_t)8  * cache[i-1]
          - (seq_t)16 * cache[i-2]
          + (seq_t)10 * cache[i-3]
          -          cache[i-4];
    }

    /* Update length of filled portion. */
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
