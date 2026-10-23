/*
 * A135259  a(n) = 3*A131666(n) - A131666(n+1).
 * https://oeis.org/A135259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*a(n-1) - a(n-3) + 2*a(n-4) Initial values: a(0)=0,
 *            a(1)=-1, a(2)=2, a(3)=1 Recurrence is valid for n ≥ 4
 * Range:     i8 n <= 10, i16 n <= 18, i32 n <= 34, i64 n <= 66, i128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t  cache_size = 0;

/* Ensure the cache holds at least indices 0…n.  Used lazily. */
static void ensure_cache(idx_t n)
{
    if (cache_size == 0) {
        /* Initialise the first four values. */
        cache_size = 4;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = -1;
        cache[2] = 2;
        cache[3] = 1;
    }

    if (n < cache_size) return;

    /* Grow the cache to at least n+1 entries, doubling each time. */
    idx_t new_size = cache_size * 2;
    if (new_size <= n) new_size = n + 1;
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    cache_size = new_size;

    /* Fill in missing terms using the recurrence. */
    for (idx_t i = 4; i < cache_size; ++i) {
        /* For i ≥ 4 the recurrence holds. */
        cache[i] = 2 * cache[i-1] - cache[i-3] + 2 * cache[i-4];
    }
}

seq_t a(idx_t n)
{
    /* Ensure all needed terms are cached. */
    ensure_cache(n);
    return cache[n];
}
