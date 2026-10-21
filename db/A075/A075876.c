/*
 * A075876  Values of m for which A075825(m) = 1.
 * https://oeis.org/A075876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A075876 – Values of m for which A075825(m)=1.
    Sequence offset: 1 (a(1) is first term).
    Recurrence (n ≥ 5):  a(n) = 4*a(n-1) + 4
    Initial values:     a(1)=0, a(2)=2, a(3)=4, a(4)=12 */
 
#include <stdlib.h>
 
seq_t a(idx_t n)
{
    /* Static cache to make repeated calls fast and order‑independent. */
    static seq_t *cache = NULL;       /* array of terms, cache[0] = a(1) */
    static size_t cached_len = 0;     /* number of terms stored in cache      */
    static size_t cache_cap  = 0;     /* allocated capacity */
 
    /* Initialise cache with the first four terms on the first call. */
    if (!cache) {
        cache_cap  = 8;                      /* start with 8 slots */
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)0;                 /* a(1) */
        cache[1] = (seq_t)2;                 /* a(2) */
        cache[2] = (seq_t)4;                 /* a(3) */
        cache[3] = (seq_t)12;                /* a(4) */
        cached_len = 4;
    }
 
    /* If the requested term is already cached, return it immediately. */
    if (n <= cached_len)
        return cache[n - 1];
 
    /* Grow the cache in powers of two until it can hold the n‑th term. */
    while (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap << 1;           /* double */
        if (new_cap == 0) new_cap = 1;            /* safety for overflow */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                               /* allocation failure */
            /* If allocation fails, fall back to direct computation. */
            cache = NULL;
            cached_len = 0;
            cache_cap = 0;
            /* compute from scratch */
            seq_t val;
            if (n == 1) return (seq_t)0;
            if (n == 2) return (seq_t)2;
            if (n == 3) return (seq_t)4;
            if (n == 4) return (seq_t)12;
            val = (seq_t)12;
            for (idx_t i = 5; i <= n; ++i)
                val = val * 4 + 4;
            return val;
        }
        cache = tmp;
        cache_cap = new_cap;
    }
 
    /* Compute missing terms up to n, starting from the last cached value. */
    for (size_t i = cached_len + 1; i <= (size_t)n; ++i) {
        cache[i - 1] = cache[i - 2] * (seq_t)4 + (seq_t)4;
    }
    cached_len = (size_t)n;
    return cache[n - 1];
}
