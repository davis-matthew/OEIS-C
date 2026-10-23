/*
 * A122048  a(n) = (n-2)*a(n-2) - a(n-3), with a(0)=0, a(1)=1, a(2)=2.
 * https://oeis.org/A122048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 13, i32 n <= 21, i64 n <= 35, i128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A122048
   Recurrence: a(0)=0, a(1)=1, a(2)=2
   a(n) = (n-2)*a(n-2) - a(n-3)  for n ≥ 3 */

#include <stdlib.h>

/* External linkage required by the build system */
seq_t a(idx_t n)
{
    /* Static cache to store already computed values */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;   /* number of values stored */
    static size_t cache_cap  = 0;   /* allocated capacity */

    /* Initialize cache with the first three terms */
    if (cache == NULL) {
        cache_cap = 3;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();                      /* escape on memory failure */
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 2;
        cache_size = 3;
    }

    /* If the requested index is already cached, return it immediately */
    if (n < (idx_t)cache_size) {
        return cache[(size_t)n];
    }

    /* Ensure enough capacity to store up to index n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 3;
        while (new_cap <= (size_t)n) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                        /* escape on memory failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute the sequence values from the current cache size up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* recurrence: a(i) = (i-2)*a(i-2) - a(i-3) */
        cache[(size_t)i] = ((seq_t)(i - 2)) * cache[(size_t)(i - 2)] - cache[(size_t)(i - 3)];
    }

    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
