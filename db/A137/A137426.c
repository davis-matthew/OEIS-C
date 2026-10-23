/*
 * A137426  a(n)=-a(n-1)+2a(n-3).
 * https://oeis.org/A137426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) + 2*a(n-3), a(0)=0, a(1)=1, a(2)=-3
 * Range:     i8 n <= 13, i16 n <= 29, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* initialize the cache with the first three terms */
    if (cache_size == 0) {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* out‑of‑memory fallback */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)(-3);
    }

    /* extend the cache if the requested index is beyond the current size */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* out‑of‑memory fallback */
        cache = tmp;
        for (size_t i = cache_size; i <= n; ++i) {
            /* recurrence: a(i) = -a(i-1) + 2*a(i-3) */
            seq_t val = -cache[i - 1] + (seq_t)2 * cache[i - 3];
            cache[i] = val;
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
