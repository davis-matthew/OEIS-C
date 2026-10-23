/*
 * A162743  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition of A162743:  Number of reduced words of length n in the Coxeter
   group on 6 generators with relations (S_i)^2 = (S_i S_j)^3 = I.
   The generating function is (t^3 + 2t^2 + 2t + 1)/(10t^3 - 4t^2 - 4t + 1),
   which yields the linear recurrence
        a(0)=1,  a(1)=6,  a(2)=30,  a(3)=135,
        a(n)=4*a(n-1)+4*a(n-2)-10*a(n-3)   for n≥4. */

#include <stdlib.h>

/* Static cache for previously computed values. */
static seq_t *cache = NULL;      /* array of cached values */
static size_t cache_cap = 0;     /* allocated capacity (in elements) */
static size_t cache_len = 0;     /* number of valid elements stored (<= cache_cap) */

/* Ensure the cache can hold at least 'needed' elements (needed ≥ 1). */
static void
ensure_capacity(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap;
        while (new_cap < needed) new_cap <<= 1;   /* grow geometrically */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Initialize cache with the first four terms if not already done. */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1;   /* a(0) */
        cache[1] = 6;   /* a(1) */
        cache[2] = 30;  /* a(2) */
        cache[3] = 135; /* a(3) */
        cache_len = 4;
    }

    if (n < cache_len)
        return cache[(size_t)n];

    /* Extend cache to accommodate up to index n. */
    ensure_capacity((size_t)n + 1);

    /* Compute missing terms using the recurrence. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val;
        if (i == 0)          val = 1;
        else if (i == 1)     val = 6;
        else if (i == 2)     val = 30;
        else if (i == 3)     val = 135;
        else                 /* i ≥ 4 */
            val = 4 * cache[i - 1]
                + 4 * cache[i - 2]
                - 10 * cache[i - 3];
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
