/*
 * A033657  Trajectory of 23 under map x->x + (x-with-digits-reversed).
 * https://oeis.org/A033657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 23; a(n) = a(n-1) + reverse_decimal(a(n-1)) for n > 0
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/reverse_decimal.h"  /* provides seq_t reverse_decimal(seq_t) */

static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of elements stored (>=1 when cache initialized) */
static size_t cache_cap = 0;   /* allocated capacity */

/* Ensure that cache has values up to index n inclusive. */
static void ensure_cache(idx_t n)
{
    /* First time: allocate and initialize a(0) = 23 */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) abort();          /* unrecoverable allocation failure */
        cache[0] = 23;                /* a(0) */
        cache_len = 1;
    }

    /* Nothing to do if we already have enough terms */
    if (n < cache_len)
        return;

    /* Grow the allocation until it can hold indices up to n */
    size_t new_cap = cache_cap;
    while (new_cap <= (size_t)n) {
        new_cap = new_cap ? new_cap * 2 : 1;
    }
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp) abort();
    cache = newp;
    cache_cap = new_cap;

    /* Compute missing terms iteratively */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t rev  = reverse_decimal(prev);
        cache[i]   = prev + rev;      /* a(i) = a(i-1) + reverse_decimal(a(i-1)) */
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
