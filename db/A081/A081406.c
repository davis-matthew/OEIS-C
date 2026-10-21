/*
 * A081406  a(n) = (n+1)*a(n-3) for n > 2, a(0) = a(1) = a(2) = 1.
 * https://oeis.org/A081406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*a(n-3) for n > 2; a(0) = a(1) = a(2) = 1
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 26, u64 n <= 44, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the OEIS sequence A081406.
 * idx_t and seq_t are defined by the build environment.
 */
seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* current capacity */

    /* ensure the cache starts with the first three terms */
    if (cache_size == 0) {
        cache = (seq_t*)calloc(3, sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* out of memory */
        cache[0] = cache[1] = cache[2] = (seq_t)1;
        cache_size = 3;
    }

    /* if n already computed, return it */
    if (n < cache_size) {
        return cache[n];
    }

    /* grow cache to accommodate n */
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) return (seq_t)0;  /* out of memory */
    cache = new_cache;

    /* compute missing terms from cache_size up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t factor = (seq_t)(i + (idx_t)1);  /* i+1 converted to seq_t */
        cache[i] = factor * cache[i - 3];
    }

    cache_size = new_size;
    return cache[n];
}
