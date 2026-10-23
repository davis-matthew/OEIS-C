/*
 * A018207  Expansion of 1/((1-3x)(1-10x)(1-12x)).
 * https://oeis.org/A018207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*a(n-1) - 186*a(n-2) + 360*a(n-3) a(0) = 1, a(1) = 25,
 *            a(2) = 439
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of entries stored */
static size_t cache_cap  = 0;   /* allocated capacity */

/* Ensure the cache can hold at least n+1 entries */
static void ensure_capacity(idx_t n)
{
    if (cache_cap > n) return;
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) with memoisation; works for any n in any order */
seq_t a(idx_t n)
{
    /* initialise cache with the base cases */
    if (cache_size == 0) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)25;
        cache[2] = (seq_t)439;
        cache_size = 3;
    }

    if (n < cache_size) return cache[(size_t)n];

    ensure_capacity(n);

    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        /* recurrence: a(i) = 25*a(i-1) - 186*a(i-2) + 360*a(i-3) */
        cache[i] =
            (seq_t)25 * cache[i-1]
          - (seq_t)186 * cache[i-2]
          + (seq_t)360 * cache[i-3];
    }

    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
