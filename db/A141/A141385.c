/*
 * A141385  a(n) = 7*a(n-1) - 9*a(n-2) + a(n-3) with a(0)=3, a(1)=7, a(2)=31.
 * https://oeis.org/A141385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 7*a(n-1) - 9*a(n-2) + a(n-3) a(0) = 3, a(1) = 7, a(2) =
 *            31
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamic cache of values */
static size_t cache_size = 0;        /* number of entries allocated */

/* Ensure the cache contains values up to n (inclusive) */
static void extend_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (cache_size >= needed)
        return;

    seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!new_cache)
        exit(1);                    /* fatal memory error */
    cache = new_cache;

    /* Initialize base terms if they are not yet stored */
    if (cache_size == 0) {
        cache[0] = (seq_t)3;
        if (needed > 1) cache[1] = (seq_t)7;
        if (needed > 2) cache[2] = (seq_t)31;
    } else {
        if (cache_size <= 1 && needed > 1) cache[1] = (seq_t)7;
        if (cache_size <= 2 && needed > 2) cache[2] = (seq_t)31;
    }

    /* Compute any new terms using the recurrence */
    for (size_t k = cache_size; k <= n; ++k) {
        if (k == 0) { cache[0] = (seq_t)3; continue; }
        if (k == 1) { cache[1] = (seq_t)7; continue; }
        if (k == 2) { cache[2] = (seq_t)31; continue; }

        /* a(k) = 7*a(k-1) - 9*a(k-2) + a(k-3) */
        cache[k] = 7 * cache[k-1] - 9 * cache[k-2] + cache[k-3];
    }
    cache_size = needed;
}

/* Public function requested by the problem */
seq_t a(idx_t n)
{
    extend_cache(n);
    return cache[(size_t)n];
}
