/*
 * A124166  a(n) = 100*(10^n - 1)/9.
 * https://oeis.org/A124166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 100 * (10^n - 1) / 9
 * Recurrence: a(0) = 0, a(1) = 100, a(n) = 11 * a(n-1) - 10 * a(n-2)  (n ≥ 2)
 *
 * This function caches computed values so that any order of calls works.
 * It is valid C11 and C++17.  
 * For C++ builds (where seq_t may be an arbitrary‑precision class) the
 * cache buffer is allocated with calloc/malloc and new'd objects are
 * constructed with placement‑new.  For C builds the buffer is simply
 * allocated with calloc.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>   /* for placement‑new in C++ builds */
#endif

/* External types provided by the build system */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms */
    static seq_t *cache = NULL;          /* pointer to allocated array */
    static size_t cache_capacity = 0;    /* allocated capacity (in elements) */
    static idx_t  cache_length   = 0;    /* number of terms stored */

    /* Base cases directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)100;

    /* Initialise the cache on the first call */
    if (cache == NULL) {
        cache_capacity = 2;
        /* Allocated buffer for the two base cases */
#ifdef __cplusplus
        cache = (seq_t*)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) abort();
        /* Construct the elements (needed for arbitrary‑precision types) */
        for (size_t i = 0; i < cache_capacity; ++i) {
            new(&cache[i]) seq_t();
        }
#else
        cache = (seq_t*)calloc(cache_capacity, sizeof(seq_t));
        if (!cache) abort();
#endif
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)100;
        cache_length = 2;
    }

    /* Return cached value if already computed */
    if (n < cache_length) {
        return cache[n];
    }

    /* Grow the cache to at least index n */
    size_t needed = (size_t)n + 1;
    if (cache_capacity < needed) {
        size_t new_cap = cache_capacity ? cache_capacity * 2 : 1;
        while (new_cap < needed) new_cap <<= 1;

        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
#ifdef __cplusplus
        /* Construct any newly allocated elements */
        for (size_t i = cache_capacity; i < new_cap; ++i) {
            new(&tmp[i]) seq_t();
        }
#endif
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_length; i <= n; ++i) {
        /* a(i) = 11*a(i-1) - 10*a(i-2) */
        seq_t val = cache[i - 1] * (seq_t)11 - cache[i - 2] * (seq_t)10;
        cache[i] = val;
    }
    cache_length = n + 1;

    return cache[n];
}
