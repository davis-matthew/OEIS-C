/*
 * A054116  a(n) = T(n,n-1), array T as in A054115.
 * https://oeis.org/A054116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A054116: a(1)=1, a(n)=Σ_{k=2..n} k!       (n≥2) */
/* Computes the sequence for any n≥1 using a static cache.  */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache of a(i) values, 1‑based indexing.  */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* number of used slots (index+1)   */
    static size_t cache_cap = 0;  /* allocated capacity */
    static seq_t factorial = 0;   /* (current_idx)!   */
    static idx_t factorial_idx = 0;
    static bool initialized = false;

    /* first call – initialise cache for 1,2 */
    if (!initialized) {
        cache_cap = 3;                     /* indices 0,1,2 */
        cache_len  = 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)0;               /* unused */
        cache[1] = (seq_t)1;               /* a(1) */
        cache[2] = (seq_t)2;               /* a(2) = 2! */
        factorial = (seq_t)2;              /* 2!  */
        factorial_idx = 2;                 /* index of last factorial */
        initialized = true;
    }

    /* Return cached value if already computed */
    if (n < cache_len)
        return cache[(size_t)n];

    /* Grow the cache to fit up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* ensure capacity */
        if (i >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 3;
            while (new_cap <= i) new_cap *= 2;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            cache_cap = new_cap;
        }
        /* compute factorial for i */
        factorial *= (seq_t)i;             /* (i−1)! * i = i!  */
        cache[i] = cache[i-1] + factorial;/* a(i) = a(i-1) + i! */
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
