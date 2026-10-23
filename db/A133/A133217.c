/*
 * A133217  Indices of decagonal numbers (A001107) that are also triangular
 *          (A000217).
 * https://oeis.org/A133217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(1)=0, a(2)=1, a(3)=2, a(4)=20, a(5)=55,
 *          for n > 5: a(n) = 34*a(n-2) - a(n-4) - 12
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not defined, but safe guard */

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    /* base values */
    const seq_t init[5] = {(seq_t)0, (seq_t)1, (seq_t)2, (seq_t)20, (seq_t)55};

    /* initialise cache up to 5 if needed */
    if (cache == NULL || cache_cap < 5)
    {
        size_t new_cap = (size_t)8; /* some room for growth */
        if (new_cap < 5) new_cap = 5;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0; /* out of memory – return 0 as a safe default */

        /* copy base values */
        for (size_t i = 0; i < 5; ++i)
            new_cache[i] = init[i];
        /* remaining up to new_cap are zero-initialised by malloc */

        cache = new_cache;
        cache_cap = new_cap;
    }

    /* expand cache if required */
    if (n > cache_cap)
    {
        size_t new_cap = cache_cap;
        while (n > new_cap)
            new_cap *= 2;          /* double until enough */

        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0;   /* out of memory */

        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute values up to n if not already done */
    idx_t i = 5; /* 0‑based index of next position to compute: corresponds to a(6) */
    while ((idx_t)(i + 1) <= n)
    {
        idx_t idx = i + 1;                /* sequence index (1‑based) */
        seq_t val = (seq_t)34 * cache[idx - 3] - cache[idx - 5] - (seq_t)12;
        cache[idx - 1] = val;
        ++i;
    }

    return cache[n - 1];
}
