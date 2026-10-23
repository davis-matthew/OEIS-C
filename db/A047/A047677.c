/*
 * A047677  Row 2 of square array defined in A047675: 2*n!*(n+1)!.
 * https://oeis.org/A047677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * n! * (n+1)!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* array of previously computed values        */
    static idx_t  cache_max = 0;         /* largest n that has been computed          */
    static int    initialized = 0;       /* initialization flag                        */

    /* --- Initialization ---------------------------------------------- */
    if (!initialized) {
#ifdef __cplusplus
        cache = new seq_t[1];
#else
        cache = (seq_t *)malloc(sizeof(seq_t));
#endif
        if (!cache) return (seq_t)0;     /* unlikely to happen */
        cache[0] = (seq_t)2;             /* a(0) = 2 */
        cache_max = 0;
        initialized = 1;
    }

    /* --- Grow cache if necessary ------------------------------------ */
    if (n > cache_max) {
        idx_t new_size = n + 1;
        seq_t *new_cache;

#ifdef __cplusplus
        new_cache = new seq_t[new_size];
        for (idx_t i = 0; i <= cache_max; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
        cache = new_cache;
#else
        new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!new_cache) return (seq_t)0;   /* fallback on OOM */
        cache = new_cache;
#endif

        /* Compute new terms iteratively using the recurrence */
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_t prev   = cache[i - 1];
            seq_t mult1  = (seq_t)i;
            seq_t mult2  = (seq_t)(i + 1);
            seq_t val    = prev * mult1;
            val          *= mult2;            /* val = prev * i * (i+1) */
            cache[i] = val;
        }
        cache_max = n;
    }

    return cache[n];
}
