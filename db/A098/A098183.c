/*
 * A098183  a(n) = 3*a(n-1) + a(n-3), a(0) = 1, a(1) = 1, a(2) = 4.
 * https://oeis.org/A098183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-3), a(0)=1, a(1)=1, a(2)=4
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Dynamic cache for already computed terms */
static seq_t *cache = NULL;        /* array of sequence values */
static size_t cache_size = 0;      /* current allocated size */
static size_t last_computed = 0;   /* highest index that has been filled */

/* Ensure that the cache can hold indices up to n */
static void ensure_cache(size_t n)
{
    if (n < cache_size)
        return;
    size_t new_size = (cache_size ? cache_size : 4);
    while (new_size <= n)
        new_size <<= 1;           /* double until large enough */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    /* If realloc fails, we keep the old cache unchanged; the caller will
       retry on the next call. */
    if (tmp) {
        cache = tmp;
        cache_size = new_size;
    }
}

/* Initialise the cache with the first three terms (0‑based indices). */
static void init_cache(void)
{
    if (cache)
        return;                   /* already initialised */

    ensure_cache(2);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)4;
    last_computed = 2;
}

/* Compute all terms up to (and including) index n. Assumes n >= last_computed. */
static void compute_up_to(size_t n)
{
    if (n <= last_computed)
        return;
    for (size_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = (seq_t)3 * cache[i-1] + cache[i-3];
    }
    last_computed = n;
}

/* Public entry point – returns a(n) for any n >= 0. */
seq_t a(idx_t n)
{
    init_cache();
    size_t idx = (size_t)n;

    if (idx >= cache_size)
        ensure_cache(idx);
    compute_up_to(idx);
    return cache[idx];
}
