/*
 * A173804  a(n) = (5*10^n - 41)/9 for n > 0.
 * https://oeis.org/A173804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) + 41 (a(1) = 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* memoization cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of elements currently stored */
static size_t cache_cap = 0;   /* allocated capacity */

/* Ensure the cache can hold up to n terms */
static void
ensure_cache(idx_t n)
{
    if ((size_t)n <= cache_len) return;
    size_t new_cap = (cache_cap == 0) ? 1 : cache_cap;
    while (new_cap < (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n < 1 but harmless */
    ensure_cache(n);

    /* Terms already cached? */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

    /* start from the last cached value (or from 1 if none) */
    seq_t prev = (cache_len == 0) ? 1 : cache[cache_len - 1];
    if (cache_len == 0) {           /* initialise a(1) */
        cache[0] = 1;
        cache_len = 1;
        prev = 1;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        prev = 10 * prev + 41;
        cache[cache_len] = prev;
        cache_len++;
    }
    return cache[n - 1];
}
