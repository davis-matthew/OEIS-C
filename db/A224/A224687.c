/*
 * A224687  Number of (n+4) X 9 0..1 matrices with each 5 X 5 subblock
 *          idempotent.
 * https://oeis.org/A224687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 18, u32 n <= 58, u64 n <= 137, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A224687
    Formula:  for n > 12
        a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4) + a(n-5)
               - 3*a(n-6) + 3*a(n-7) - a(n-8)
    Initial values: a(1..12) = 4405, 1938, 2136, 2429, 2700, 2926,
                    3540, 4514, 5773, 7247, 8885, 11074
    (a(0) is unused, stored as 0 for convenience)
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 1‑based cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;            /* allocated slots (>= n+1) */
    static idx_t cache_size = 0;            /* largest n computed */

    /* static initial values (index 0 is dummy) */
    static const seq_t init[13] = {
        /* 0th dummy */                      (seq_t)0,
        /* a(1)..a(12) */   (seq_t)4405, (seq_t)1938, (seq_t)2136,
        (seq_t)2429, (seq_t)2700, (seq_t)2926, (seq_t)3540,
        (seq_t)4514, (seq_t)5773, (seq_t)7247, (seq_t)8885,
        (seq_t)11074
    };

    /* initialise cache with the first 13 slots (0..12) if it does not exist yet */
    if (!cache) {
        cache_cap = 13;   /* indices 0..12 */
#ifdef __cplusplus
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) abort();   /* out of memory */
#endif
        for (size_t i = 0; i < cache_cap; ++i)
            cache[i] = init[i];
        cache_size = 12;
    }

    /* grow cache if needed */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = n + 1;
        seq_t *new_cache;
#ifdef __cplusplus
        new_cache = new seq_t[new_cap];
#else
        new_cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
        if (!new_cache) abort();
#endif
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    if (n > cache_size) {
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t t = 4 * cache[i - 1];
            t    -= 6 * cache[i - 2];
            t    += 4 * cache[i - 3];
            t    -= cache[i - 4];
            t    += cache[i - 5];
            t    -= 3 * cache[i - 6];
            t    += 3 * cache[i - 7];
            t    -= cache[i - 8];
            cache[i] = t;
        }
        cache_size = n;
    }

    return cache[n];
}
