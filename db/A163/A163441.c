/*
 * A163441  Number of reduced words of length n in Coxeter group on 16
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1)+14*a(n-2)+14*a(n-3)+14*a(n-4)-105*a(n-5) for n
 *            >= 6, with base values a(0)=1, a(1)=16, a(2)=240, a(3)=3600,
 *            a(4)=54000, a(5)=809880
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;        /* array of computed terms   */
    static size_t cache_size = 0;           /* allocated size in entries */
    static idx_t last_filled = 5;           /* highest index computed   */

    /* initial values for the recurrence */
    static const seq_t init[6] = {
        (seq_t)1ULL,      /* a(0) */
        (seq_t)16ULL,     /* a(1) */
        (seq_t)240ULL,    /* a(2) */
        (seq_t)3600ULL,   /* a(3) */
        (seq_t)54000ULL,  /* a(4) */
        (seq_t)809880ULL  /* a(5) */
    };

    /* Base cases – no need to touch the cache. */
    if (n < 6)
        return init[n];

    /* Allocate and initialise the cache if not done yet. */
    if (!cache)
    {
        size_t needed = 6;
        while (needed <= (size_t)n)
            needed <<= 1;          /* round up to a power of two */

        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!cache)                /* allocation failed – return zero */
            return (seq_t)0;

        for (size_t i = 0; i < 6; ++i)
            cache[i] = init[i];

        last_filled = 5;
        cache_size  = needed;
    }

    /* Resize the cache if the requested index exceeds the allocation. */
    if ((size_t)n >= cache_size)
    {
        size_t new_cap = cache_size ? cache_size << 1 : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;

        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache)                    /* allocation failed */
            return (seq_t)0;

        /* Zero the new part (optional, but keeps cache clean). */
        for (size_t i = cache_size; i < new_cap; ++i)
            cache[i] = 0;

        cache_size = new_cap;
    }

    /* If the value is already computed, return it. */
    if (n <= last_filled)
        return cache[n];

    /* Compute from last_filled+1 up to n using the linear recurrence. */
    for (idx_t i = last_filled + 1; i <= n; ++i)
    {
        /* recurrence only holds for i >= 6; on i==6 the formula is
           exactly the same as for larger indices because the numerator
           contributes only up to degree 5, which has already been baked
           into the initial value a(5). */
        cache[i] = (seq_t)(14 * cache[i - 1]
                           + 14 * cache[i - 2]
                           + 14 * cache[i - 3]
                           + 14 * cache[i - 4]
                           - 105 * cache[i - 5]);
    }

    last_filled = n;
    return cache[n];
}
