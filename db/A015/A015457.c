/*
 * A015457  Generalized Fibonacci numbers.
 * https://oeis.org/A015457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) + a(n-2), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache to support arbitrary call order */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* grow capacity if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        /* recurrence: a(i) = 11*a(i-1) + a(i-2) */
        cache[(size_t)i] =
            (seq_t)11 * cache[(size_t)(i - 1)] + cache[(size_t)(i - 2)];
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
