/*
 * A010845  a(n) = 3*n*a(n-1) + 1, a(0) = 1.
 * https://oeis.org/A010845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*a(n-1) + 1, a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function remembers previously computed values, so it works
   efficiently even if calls come in arbitrary order. */
#include <stdlib.h>  /* for malloc, realloc, free */

static seq_t *cache = NULL;      /* array of computed terms */
static idx_t cache_cap = 0;      /* current allocated capacity */
static idx_t cache_used = 0;     /* next free index (number of terms stored) */

/* Grow the cache so that it can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;

    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < n + 1)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* realloc may return NULL only if size == 0; the caller never asks for
       zero size, so no failure handling is needed here. */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return a(n) for OEIS sequence A010845. */
seq_t a(idx_t n)
{
    /* Seed the cache with the first term if empty. */
    if (cache_used == 0) {
        ensure_cache(0);
        cache[0] = (seq_t)1;    /* a(0) = 1 */
        cache_used = 1;
    }

    /* If already computed, return it immediately. */
    if (n < cache_used)
        return cache[n];

    /* Ensure there is room for all needed indices. */
    ensure_cache(n);

    /* Compute missing terms up to n. */
    for (idx_t i = cache_used; i <= n; ++i) {
        seq_t prev = cache[i - 1];          /* a(i-1) */
        seq_t prod = (seq_t)i * prev;       /* i * a(i-1) */
        seq_t term = (seq_t)3 * prod + (seq_t)1; /* 3*i*a(i-1) + 1 */
        cache[i] = term;
    }

    cache_used = n + 1;
    return cache[n];
}
