/*
 * A046179  Indices of hexagonal numbers that are also pentagonal.
 * https://oeis.org/A046179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A046179:  Indices of hexagonal numbers that are also pentagonal
   Recurrence:  a(1)=1, a(2)=143,
                a(n)=194*a(n-1)-a(n-2)-48   (n≥3).  Indexes start at 1. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;   /* cache array */
    static size_t cache_size = 0; /* number of terms currently stored */
    static size_t cache_cap  = 0; /* allocated capacity in elements */

    if (n < 1)                 /* undefined, guard */
        return (seq_t)0;

    /* Ensure capacity for at least n terms */
    if (cache_cap < (size_t)n) {
        size_t new_cap = (cache_cap == 0) ? 2 : cache_cap;
        while (new_cap < (size_t)n)
            new_cap <<= 1;

#if defined(__cplusplus)
        seq_t *tmp = new seq_t[new_cap];
        for (size_t i = 0; i < cache_size; ++i)
            tmp[i] = cache[i];
        delete[] cache;
#else
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failure */
        cache = tmp;
#endif
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first two terms if they are missing */
    if (cache_size < 1) {
        cache[0] = (seq_t)1;     /* a(1) */
        cache_size = 1;
    }
    if (cache_size < 2) {
        cache[1] = (seq_t)143;   /* a(2) */
        cache_size = 2;
    }

    /* Compute missing terms up to n */
    for (size_t i = cache_size; i < (size_t)n; ++i) {
        /* i corresponds to a(i+1) */
        cache[i] =
            (seq_t)194 * cache[i - 1] - cache[i - 2] - (seq_t)48;
    }
    cache_size = (size_t)n;

    return cache[n - 1];
}
