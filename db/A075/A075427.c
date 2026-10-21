/*
 * A075427  a(0) = 1; a(n) = a(n-1)+1 if n is even, otherwise a(n) = 2*a(n-1).
 * https://oeis.org/A075427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(0) = 1; a(n) = a(n-1)+1 if n is even, a(n) = 2*a(n-1) if n is
 *            odd
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 62, u64 n <= 126,
 *            u128 n <= 254, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for computed values */
static seq_t *cache = NULL;      /* array holding computed terms */
static idx_t cache_size = 0;     /* number of terms currently stored (cache_size == n+1 for last cached n) */
static idx_t cache_cap  = 0;     /* capacity of the cache array */

/* Ensure that the cache contains at least terms up to and including n */
static void
ensure_cache(idx_t n)
{
    if (n < cache_size) return;          /* already computed */

    /* Grow the cache if necessary */
    idx_t needed = n + 1;
    if (cache_cap < needed) {
        /* double capacity until big enough, starting from 16 */
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < needed) new_cap *= 2;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) { /* allocation failed – abort in an unlikely case */
            abort();
        }
        if (cache) {
            for (idx_t i = 0; i < cache_size; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        cache[i] = (i & 1) ? prev * 2 : prev + 1;
    }
    cache_size = needed;
}

seq_t
a(idx_t n)
{
    /* Lazy initialization of the cache */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();     /* allocation failure */
        cache[0] = 1;
        cache_size = 1;
    }

    if (n >= cache_size) {
        ensure_cache(n);
    }
    return cache[n];
}
