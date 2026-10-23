/*
 * A160225  Numerator of Hermite(n, 2/29).
 * https://oeis.org/A160225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 29^n * Hermite(n, 2/29) Recurrence: a(0)=1, a(1)=4,
 *            a(n+1)=4*a(n) - 2*841*n*a(n-1) for n ≥ 1
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of initialized elements        */
static size_t cache_cap   = 0;   /* allocated capacity (elements or 0)   */

/* Ensure the cache can hold at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = newp;
    cache_cap = new_cap;
}

/* Return the nth term a(n). */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;

    /* Initialise the first two terms if the cache is empty. */
    if (cache_size == 0) {
        grow_cache(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache_size = 2;
    }

    /* If value already computed, return it. */
    if (idx < cache_size)
        return cache[idx];

    /* Grow the cache to accommodate the requested index. */
    grow_cache(idx + 1);

    /* Compute missing terms using the recurrence. */
    for (size_t i = cache_size; i <= idx; ++i) {
        seq_t cur   = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        /* mult = 2 * (i-1) * 841 * prev2 */
        seq_t mult = (seq_t)(2 * (i - 1)) * (seq_t)841 * prev2;
        cache[i] = (seq_t)4 * cur - mult;
    }

    cache_size = idx + 1;
    return cache[idx];
}
