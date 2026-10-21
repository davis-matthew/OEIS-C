/*
 * A085327  Duplicate of A069403.
 * https://oeis.org/A085327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(0)=1, a(1)=3, a(n)=3*a(n-1)-a(n-2)+1 for n>=2
 * Range:     i8 n <= 4, i16 n <= 10, i32 n <= 14, i64 n <= 14, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation stores already computed terms in a static cache.
 * The cache is increased only when a larger index is requested, so
 * repeated calls with arbitrary ordering of n are efficient.  All
 * operations are performed with type seq_t, so the implementation works
 * for arbitrary‑precision builds as well. */

#include <stdlib.h>   /* for malloc, realloc */

static seq_t *cache = NULL;      /* array of computed terms          */
static size_t cache_len = 0;     /* number of terms currently stored  */
static size_t cache_cap = 0;     /* allocated capacity                */

/* Ensure the cache can hold (new_cap) elements. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    /* realloc may return NULL; in that case we keep the old cache. */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) /* allocation failed – keep old cache */
        return;
    cache = tmp;
    cache_cap = new_cap;
}

/* Return a(n).  The cache is filled incrementally so that future
 * calls with smaller or larger n need not recompute all terms. */
seq_t a(idx_t n)
{
    /* On first call initialise the first two terms (and capacity). */
    if (cache_len == 0) {
        grow_cache(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache_len = 2;
    }

    /* Extend the cache if necessary. */
    if ((size_t)n >= cache_len) {
        grow_cache((size_t)n + 1);          /* ensure buffer for n */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i == 0) {
                /* already initialised */
                continue;
            } else if (i == 1) {
                /* already initialised */
                continue;
            } else {
                /* a(i) = 3*a(i-1) - a(i-2) + 1 */
                cache[i] = (seq_t)3 * cache[i - 1]
                         - cache[i - 2]
                         + (seq_t)1;
            }
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
