/*
 * A234264  Number of (n+1) X (6+1) 0..2 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 2
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 6*a(n-2) - 24*a(n-3) + 4*a(n-4) + 36*a(n-5) -
 *            24*a(n-6) Initial values: a(1)=1336, a(2)=1690, a(3)=2324,
 *            a(4)=3616, a(5)=6076, a(6)=11140
 * Range:     u8 none, u16 n <= 8, u32 n <= 22, u64 n <= 47, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build environment */
seq_t a(idx_t n)
{
    /* Static cache: cache[i] = a(i) for i>=1, cache[0] unused */
    static seq_t *cache      = NULL;   /* allocated array */
    static size_t capacity   = 0;      /* size of allocated array */
    static size_t computed   = 0;      /* maximum n that has been computed */
    static bool   init_done  = false;  /* true after the first 6 terms are stored */

    /* Sequence is defined only for n >= 1 */
    if (n < 1)
        return (seq_t)0;

    /* Ensure the cache has room for index n (index 0 unused), but at least 7 slots */
    size_t needed = (size_t)n + 1;
    if (needed < 7) needed = 7;
    if (capacity < needed)
    {
        size_t new_cap = needed;
        /* increase capacity (double it) for future calls */
        if (new_cap < capacity * 2)
            new_cap = capacity ? capacity * 2 : 8;
        if (new_cap < needed) new_cap = needed;

#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_cap];
#else
        seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
        if (!tmp)
            return (seq_t)0; /* allocation failed */

        /* copy previously cached values */
        if (cache && capacity > 0)
        {
            for (size_t i = 0; i < capacity && i < new_cap; ++i)
                tmp[i] = cache[i];
        }
        /* free old storage */
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
        cache    = tmp;
        capacity = new_cap;
    }

    /* Initialise the first six terms once */
    if (!init_done)
    {
        cache[1] = (seq_t)1336;
        cache[2] = (seq_t)1690;
        cache[3] = (seq_t)2324;
        cache[4] = (seq_t)3616;
        cache[5] = (seq_t)6076;
        cache[6] = (seq_t)11140;
        computed = 6;
        init_done = true;
    }

    /* If the requested value is already computed, return it */
    if (n <= computed)
        return cache[(size_t)n];

    /* Compute missing values up to n */
    for (idx_t k = computed + 1; k <= n; ++k)
    {
        /* All operations are performed in seq_t; for unsigned types
           the recurrence produces a non‑negative result, so subtraction
           does not wrap around. */
        seq_t t1 = 3 * cache[(size_t)k - 1];
        t1      += 6 * cache[(size_t)k - 2];
        t1      += 4 * cache[(size_t)k - 4];
        t1      += 36 * cache[(size_t)k - 5];

        seq_t t2 = 24 * cache[(size_t)k - 3];
        t2      += 24 * cache[(size_t)k - 6];

        cache[(size_t)k] = t1 - t2;
    }
    computed = (size_t)n;
    return cache[(size_t)n];
}
