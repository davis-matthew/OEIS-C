/*
 * A135690  a(n) = a(n-2) - (a(n-1) - a(n-2)) if (n mod 2) = 0, otherwise a(n)
 *          = a(n-1) - (a(n-3) - a(n-4)), with a(0) = 0, a(1) = 1, a(2) = -1,
 *          a(3) = 2.
 * https://oeis.org/A135690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, less
 * Formula:   not stated
 * Range:     i8 n <= 21, i16 n <= 53, i32 n <= 117, i64 n <= 245,
 *            i128 n <= 501, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-2) - (a(n-1) - a(n-2))  if n mod 2 = 0
 *            a(n) = a(n-1) - (a(n-3) - a(n-4)) otherwise
 * Initial values: a(0)=0, a(1)=1, a(2)=-1, a(3)=2
 * (equivalently: a(n)=2*a(n-2)-a(n-1) for even n, a(n)=a(n-1)-a(n-3)+a(n-4) for odd n)
 */

#include <stddef.h>   /* NULL, size_t */
#include <stdlib.h>   /* malloc, realloc, free */

#ifdef __cplusplus
#include <algorithm>  /* std::copy */
#endif

/* Return the n-th term of OEIS sequence A135690.
 * The type seq_t is defined by the build and may be either a
 * fixed‑size signed integer or an arbitrary‑precision integer class.
 * idx_t is a (non‑negative) unsigned type (here uint64_t). */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;     /* allocated size (in elements)   */
    static idx_t cached_max = (idx_t)-1;   /* largest index initialized */

    /* Allocate initial cache for indices 0..3 on first call */
    if (cache == NULL) {
        /* Allocate space for four elements */
#ifdef __cplusplus
        cache = new seq_t[4];
#else
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
#endif
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = -1;
        cache[3] = 2;
        cache_cap = 4;
        cached_max = 3;
    }

    /* Expand cache if required to hold index n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        /* double until new_cap > n */
        while (new_cap <= (size_t)n) new_cap <<= 1;
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_cap];
        std::copy(cache, cache + cache_cap, new_cache);
        delete[] cache;
        cache = new_cache;
#else
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n */
    if (n > cached_max) {
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            if ((i & 1ULL) == 0) {          /* even i */
                cache[i] = 2 * cache[i - 2] - cache[i - 1];
            } else {                         /* odd i */
                cache[i] = cache[i - 1] - cache[i - 3] + cache[i - 4];
            }
        }
        cached_max = n;
    }

    return cache[n];
}
