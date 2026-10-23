/*
 * A158530  a(n) = Hermite(n,7).
 * https://oeis.org/A158530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Hermite(n,7) Recurrence (physicists' Hermite
 *            polynomials): H_0(x) = 1 H_1(x) = 2x H_n(x) = 2x H_{n-1}(x) -
 *            2(n-1) H_{n-2}(x) with x = 7, so 2x = 14. Thus a(0) = 1 a(1) =
 *            14 a(n) = 14*a(n-1) - 2*(n-1)*a(n-2) for n >= 2
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 8, i64 n <= 17, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
    /* C++ compiler */
    static seq_t *cache = nullptr;
    static size_t cache_cap = 0;
    static size_t cache_count = 0;
#else
    /* C compiler */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_count = 0;
#endif

seq_t a(idx_t n)
{
    /* Exact value for a(0) */
    if (n == 0)
        return (seq_t)1;

    /* Ensure enough space for indices 0 .. n */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
#ifdef __cplusplus
        /* Allocate a new array and copy old contents */
        seq_t *new_cache = nullptr;
        new_cache = new seq_t[needed];            /* default construction */
        if (cache) {
            for (size_t i = 0; i < cache_cap; ++i)
                new_cache[i] = cache[i];
            delete[] cache;
        }
        cache = new_cache;
        cache_cap = needed;
#else
        cache = realloc(cache, needed * sizeof(seq_t));
        cache_cap = needed;
#endif
    }

    /* Initialize base cases if not done yet */
    if (cache_count == 0) {
        cache[0] = (seq_t)1;
        cache_count = 1;
    }
    if (cache_count == 1 && n >= 1) {
        cache[1] = (seq_t)14;
        cache_count = 2;
    }

    /* Compute any missing terms up to n */
    for (size_t i = cache_count; i <= (size_t)n; ++i) {
        seq_t term = (seq_t)14 * cache[i-1]          /* 14*a(i-1) */
                   - ((seq_t)2 * (seq_t)(i-1)) * cache[i-2]; /* 2*(i-1)*a(i-2) */
        cache[i] = term;
    }

    cache_count = (size_t)n + 1;
    return cache[n];
}
