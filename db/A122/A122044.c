/*
 * A122044  a(n) = a(n-2) - (n-3)*a(n-3), with a(0)=0, a(1)=1, a(2)=2.
 * https://oeis.org/A122044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-2) - (n-3)*a(n-3), a(0)=0, a(1)=1, a(2)=2
 * Range:     i8 n <= 12, i16 n <= 18, i32 n <= 27, i64 n <= 46, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Ensure the cache is large enough for indices 0 … n */
    size_t need = (size_t)n + 1;

    if (cache_len < need) {
        size_t new_len = need;
        seq_t *new_cache = NULL;

#ifdef __cplusplus
        /* C++: allocate new array and copy old values */
        new_cache = new seq_t[new_len];
        if (!new_cache) exit(EXIT_FAILURE);
        for (size_t i = 0; i < cache_len; ++i) {
            new_cache[i] = cache[i];
        }
        delete [] cache;
#else
        /* C: realloc works for scalar types, including an arbitrary‑precision
           class that has trivial constructor/destructor in practice. */
        new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        /* old entries are preserved in the reallocated block */
#endif
        cache = new_cache;
    }

    /* Compute any new terms beyond the current cache */
    for (size_t i = cache_len; i < need; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)0;
        } else if (i == 1) {
            cache[i] = (seq_t)1;
        } else if (i == 2) {
            cache[i] = (seq_t)2;
        } else {
            seq_t mult = (seq_t)((idx_t)i - 3);
            cache[i] = cache[i - 2] - mult * cache[i - 3];
        }
    }

    cache_len = need;
    return cache[(size_t)n];
}
