/*
 * A189849  a(0)=1, a(1)=0, a(n) = 4*n*(n-1)*(a(n-1) + 2*(n-1)*a(n-2)).
 * https://oeis.org/A189849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=0, a(n) = 4*n*(n-1)*(a(n-1) + 2*(n-1)*a(n-2)) (n >=
 *            2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for memoised values (external linkage: a) */
static seq_t *cache = NULL;      /* pointer to cache array */
static size_t cache_cap = 0;     /* allocated capacity of the array */
static size_t cache_size = 0;    /* number of values currently computed */

/* Grow the cache to hold at least 'needed' elements.  Must be
 * defined at file scope, not nested inside a(). */
static void
ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        free(cache);
        exit(EXIT_FAILURE);     /* out of memory */
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t
a(idx_t n)
{
    /* Ensure we have room for indices up to n */
    ensure_cache_cap(n + 1);

    /* Initialise base values if not yet present */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache_size = 1;
    }
    if (cache_size == 1 && n >= 1) {
        cache[1] = (seq_t)0;
        cache_size = 2;
    }

    /* Compute any missing terms up to n (inclusive) */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t term_left  = cache[i - 1];
        seq_t term_right = (seq_t)2 * (seq_t)(i - 1) * cache[i - 2];
        seq_t inner      = term_left + term_right;
        seq_t factor     = (seq_t)4 * (seq_t)i * (seq_t)(i - 1);
        cache[i]         = factor * inner;
    }

    /* Update number of computed values */
    cache_size = n + 1;

    return cache[n];
}
