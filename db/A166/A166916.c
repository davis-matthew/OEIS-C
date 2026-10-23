/*
 * A166916  a(n) = 20*a(n-1) - 64*a(n-2) - 15 for n > 1; a(0) = 357, a(1) =
 *          5525.
 * https://oeis.org/A166916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 357, a(1) = 5525, a(2) = 87637 a(n) = 21*a(n-1) -
 *            84*a(n-2) + 64*a(n-3) for n >= 3 This recurrence generates the
 *            sequence A166916
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 13, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;      /* number of terms stored (cache[0..size-1]) */
static size_t cache_cap = 0;      /* capacity of allocated array */

/* Ensure the cache has capacity for at least `needed` elements. */
static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    /* Grow capacity (double each time, but at least needed). */
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache && new_cap > 0)  /* allocation failed */
        exit(1);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute all terms up to `n` (inclusive) using the recurrence. */
static void compute_up_to(idx_t n)
{
    if (n < cache_size)
        return;  /* already computed */

    ensure_capacity(n + 1);

    /* Initialise the first three terms if not already set. */
    if (cache_size == 0) {
        cache[0] = 357;
        cache_size = 1;
    }
    if (cache_size == 1) {
        cache[1] = 5525;
        cache_size = 2;
    }
    if (cache_size == 2) {
        cache[2] = 87637;
        cache_size = 3;
    }

    /* Compute remaining terms. */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = 21 * cache[i - 1] - 84 * cache[i - 2] + 64 * cache[i - 3];
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
