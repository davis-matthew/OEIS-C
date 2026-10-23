/*
 * A161474  Triple factorials n!!!: a(n) = n*a(n-3).
 * https://oeis.org/A161474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 26, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = n*a(n‑3) with a(0)=a(1)=a(2)=1 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Simple dynamic cache: recompute from 0 up to the new size each time */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Allocate or resize the cache if necessary */
    if (cache_size <= n) {
        /* Prefer regrowing to avoid frequent reallocation for small n */
        idx_t new_size = n + 1;
        seq_t *new_cache;
#ifdef __cplusplus
        new_cache = reinterpret_cast<seq_t*>(malloc((size_t)new_size * sizeof(seq_t)));
#else
        new_cache = (seq_t*)malloc((size_t)new_size * sizeof(seq_t));
#endif
        if (!new_cache)          /* allocation failure – return 0 (undefined for huge n) */
            return 0;

        /* Compute all terms up to n */
        for (idx_t i = 0; i <= n; ++i) {
            if (i < 3)
                new_cache[i] = (seq_t)1;
            else
                new_cache[i] = (seq_t)i * new_cache[i - 3];
        }

        /* Release previous cache */
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
