/*
 * A051794  a(n) = Sum_{i=n-6..n-1} (-1)^i * a(i), a(1)=1, a(2)=1, a(3)=1,
 *          a(4)=1, a(5)=1, a(6)=1.
 * https://oeis.org/A051794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, sign
 * Formula:   a(n) = Sum_{i=n-6..n-1} (-1)^i * a(i), a(1)=…=a(6)=1
 * Range:     i8 n <= 60, i16 n <= 112, i32 n <= 208, i64 n <= 402,
 *            i128 n <= 788, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For indices < 1 the behaviour
       is undefined. */
    if (n < 1)
        return 0;

    /* Base values a(1)..a(6) = 1. */
    if (n <= 6)
        return 1;

    static seq_t *cache = NULL;       /* 1‑based values stored at cache[0] */
    static size_t cache_size = 0;     /* allocated array size (elements) */
    static size_t cache_len = 0;      /* number of values already computed */

    /* Ensure the array can hold the requested index. */
    if ((size_t)n > cache_size) {
        size_t new_size = cache_size ? cache_size * 2 : 16;
        while (new_size < (size_t)n)
            new_size *= 2;
        /* Allocate or grow the array. */
        void *tmp = realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();               /* out of memory */
        cache = (seq_t*)tmp;             /* explicit cast for C++ */
        cache_size = new_size;
    }

    /* Initialise the first six terms once. */
    if (cache_len == 0) {
        for (size_t i = 0; i < 6 && i < cache_size; ++i)
            cache[i] = 1;                /* a(1)..a(6) */
        cache_len = 6;
    }

    /* Compute any missing terms up to n. */
    if ((size_t)n > cache_len) {
        for (size_t idx = cache_len + 1; idx <= (size_t)n; ++idx) {
            seq_t sum = 0;
            /* The loop runs for i = idx-6 .. idx-1 (all >= 1). */
            for (idx_t i = (idx_t)idx - 6; i <= (idx_t)idx - 1; ++i) {
                seq_t term = cache[i - 1];
                /* (-1)^i: even i -> +, odd i -> - */
                if ((i & 1ULL) == 0)
                    sum += term;
                else
                    sum -= term;
            }
            cache[idx - 1] = sum;          /* cache is 0‑based */
        }
        cache_len = n;
    }

    return cache[n - 1];
}
