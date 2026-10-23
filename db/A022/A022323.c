/*
 * A022323  a(n) = a(n-1) + a(n-2) + 1, with a(0) = 1 and a(1) = 9.
 * https://oeis.org/A022323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-2) + 1,  a(0) = 1,  a(1) = 9 (offset 0) */
#include <stdlib.h>

/* Leibniz cache to compute a(n) for any n≥0 in O(n) time per call.
   The function is thread‑unsafe but satisfies the sequence requirements. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached values */
    static size_t cache_size = 0; /* number of entries currently cached */

    /* initialise cache with the first two terms if this is the first call */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
    }

    /* Return if the term is already cached */
    if ((size_t)n < cache_size)
        return cache[n];

    /* Remember the old cache size before expanding */
    size_t old_size = cache_size;
    size_t new_size = (size_t)n + 1;

    /* Expand the cache */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (new_cache == NULL) {
        /* Allocation failed – in a real program you might handle this gracefully */
        exit(1);
    }
    cache = new_cache;
    cache_size = new_size;

    /* Compute missing terms, starting from the first index not yet present.
       Ensure the start is at least 2 so that a(0) and a(1) are already known. */
    size_t start = old_size;
    if (start < 2)
        start = 2;

    for (idx_t i = (idx_t)start; i <= n; ++i)
        cache[i] = cache[i - 1] + cache[i - 2] + (seq_t)1;

    return cache[n];
}
