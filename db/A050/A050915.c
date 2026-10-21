/*
 * A050915  a(n) = n*4^n + 1.
 * https://oeis.org/A050915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*4^n + 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Persistent cache and power of 4 */
    static seq_t *cache   = NULL;   /* array of computed a(i) */
    static seq_t pow4     = (seq_t)1;/* current 4^i  (i = last_n) */
    static idx_t last_n   = 0;      /* maximum n already computed */
    static size_t cache_cap = 0;    /* allocated capacity of cache */

    /* Initialise on first call */
    if (!cache) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) abort();          /* allocation failure */
        cache[0] = (seq_t)1;          /* a(0) = 1 */
    }

    /* If the requested value is already cached, return it. */
    if (n <= last_n) {
        return cache[(size_t)n];
    }

    /* Ensure cache has enough space.  Grow geometrically. */
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms incrementally. */
    for (idx_t i = last_n + 1; i <= n; ++i) {
        pow4 *= (seq_t)4;                     /* pow4 = 4^i  */
        cache[(size_t)i] = ((seq_t)i * pow4) + (seq_t)1;
    }

    last_n = n;
    return cache[(size_t)n];
}
