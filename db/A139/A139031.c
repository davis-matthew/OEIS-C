/*
 * A139031  Imaginary part of (4 + 3i)^n.
 * https://oeis.org/A139031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 8*a(n-1) - 25*a(n-2), a(1)=3, a(2)=24
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 13, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoization; cache[0] is unused. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* highest index that has been computed */
static idx_t cache_cap = 0;   /* capacity of the cache buffer */

/* Allocate or grow the cache so that cache[n] is defined. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len)
        return;

    if (n >= cache_cap) {
        size_t new_cap = (size_t)(n + 1);          /* include slot 0 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i == 1)
            cache[1] = (seq_t)3;
        else if (i == 2)
            cache[2] = (seq_t)24;
        else
            cache[i] = cache[i-1] * (seq_t)8 - cache[i-2] * (seq_t)25;
    }
    cache_len = n;
}

/* Public sequence function.  Offset is 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                /* undefined for n < 1, return 0 */
    ensure_cache(n);
    return cache[n];
}
