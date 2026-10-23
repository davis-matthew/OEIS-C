/*
 * A189247  Number of n X 2 array permutations with each element making zero
 *          or one king moves.
 * https://oeis.org/A189247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-1) + 12*a(n-2) - 16*a(n-3)  (n ≥ 4)
   Initial terms: a(1)=2, a(2)=24, a(3)=152 */
#include <stddef.h>
#include <stdlib.h>

/* Macros for portable allocation / deallocation */
#ifdef __cplusplus
#  define ALLOC_ARRAY(cnt)   new seq_t[(cnt)]
#  define FREE_ARRAY(ptr)    delete [] (ptr)
#else
#  define ALLOC_ARRAY(cnt)   ((seq_t*)malloc(sizeof(seq_t)*(cnt)))
#  define FREE_ARRAY(ptr)    (free((ptr)), (ptr)=NULL)
#endif

/* Static cache to store previously computed values */
static seq_t *cache      = NULL;   /* pointer to array of seq_t */
static idx_t  cache_size = 0;      /* allocated size (number of elements) */
static idx_t  last_filled = 0;     /* largest index that has a valid value */

/* Ensure the cache is big enough to hold indices up to (n+1) elements */
static void ensure_cache(idx_t n)
{
    /* we need capacity at least n+1 because indices start at 0 */
    idx_t needed = n + 1;
    if (cache == NULL) {
        /* first allocation */
        cache_size = needed > 4 ? needed : 4; /* at least 4 for safety */
        cache = ALLOC_ARRAY(cache_size);
        /* initialise the array elements to zero (C version) */
#ifndef __cplusplus
        /* For C, set to zero; for C++ objects this is no-op */
        for (idx_t i = 0; i < cache_size; ++i) { cache[i] = (seq_t)0; }
#endif
        return;
    }
    if (cache_size <= needed) {
        idx_t new_size = needed * 2 > cache_size ? needed * 2 : cache_size * 2;
        seq_t *new_cache = ALLOC_ARRAY(new_size);
#ifndef __cplusplus
        for (idx_t i = 0; i < new_size; ++i) { new_cache[i] = (seq_t)0; }
#endif
        /* copy old data */
        for (idx_t i = 0; i < cache_size; ++i) { new_cache[i] = cache[i]; }
        FREE_ARRAY(cache);
        cache = new_cache;
        cache_size = new_size;
    }
}

/* Initialise base values a(1)=2, a(2)=24, a(3)=152 */
static void init_base_values(void)
{
    if (last_filled >= 3) return;  /* already initialised */
    /* Ensure we have space for indices 1..3 */
    ensure_cache(3);
    cache[1] = (seq_t)2;
    cache[2] = (seq_t)24;
    cache[3] = (seq_t)152;
    last_filled = 3;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined for n<1, return 0 */

    /* Special case: small n handled directly after base init */
    if (n <= 3) {
        init_base_values();
        return cache[n];
    }

    /* Ensure cache is ready */
    ensure_cache(n);

    /* Initialise base terms if this is the first time */
    init_base_values();

    /* If we already computed up to n, just return it */
    if (n <= last_filled) return cache[n];

    /* Compute recurrence from max(last_filled+1, 4) up to n */
    idx_t start = last_filled + 1;
    if (start < 4) start = 4;
    for (idx_t i = start; i <= n; ++i) {
        cache[i] = 6 * cache[i - 1] + 12 * cache[i - 2] - 16 * cache[i - 3];
    }
    last_filled = n;
    return cache[n];
}
