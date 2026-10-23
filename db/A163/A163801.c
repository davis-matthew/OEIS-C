/*
 * A163801  a(n) = n - a(a(n-2)) with a(0)=0, a(1)=1.
 * https://oeis.org/A163801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 413, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0, a(1) = 1, a(2) = 2,
   a(n) = n - a(a(n-2))   for n >= 3   (OEIS A163801) */

#include <stdlib.h>
#include <stddef.h>

/* Cached sequence values */
static seq_t *cache      = NULL;   /* array of a(i) */
static size_t cache_cap = 0;       /* allocated capacity */
static idx_t  cache_max = (idx_t)-1; /* highest already computed index */

/* Ensure that the cache can hold indices up to n. */
static void ensure_capacity(idx_t n)
{
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* abort on allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return a(n) for any n >= 0.  Subsequent calls may be in any order. */
seq_t a(idx_t n)
{
    /* If already computed, return cached value. */
    if (cache_max != (idx_t)-1 && n <= cache_max)
        return cache[n];

    /* Resize cache if needed. */
    ensure_capacity(n);

    /* Start computing from the first unused index. */
    idx_t start = (cache_max == (idx_t)-1) ? 0 : cache_max + 1;

    for (idx_t i = start; i <= n; ++i) {
        seq_t val;
        if (i == 0)
            val = 0;
        else if (i == 1)
            val = 1;
        else if (i == 2)
            val = 2;
        else {
            /* a(i) = i - a(a(i-2))   */
            idx_t inner_idx = (idx_t)cache[i - 2]; /* a(i-2) is the index */
            seq_t inner = cache[inner_idx];        /* a(a(i-2)) */
            val = (seq_t)i - inner;
        }
        cache[i] = val;
    }

    cache_max = n;
    return cache[n];
}
