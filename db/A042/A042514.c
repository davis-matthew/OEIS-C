/*
 * A042514  Numerators of continued fraction convergents to sqrt(786).
 * https://oeis.org/A042514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 1570*a(n-2) - a(n-4) a(0) = 28, a(1) = 785, a(2) = 43988,
 *            a(3) = 1232449
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <new>
#endif
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Ensure the cache is large enough for n+1 entries */
    size_t needed = (size_t)n + 1;
    if (needed > cache_size)
    {
        size_t old_size = cache_size;
        /* Allocate at least 4 entries to hold the initial values */
        size_t new_size = (needed > 4) ? needed : 4;

        seq_t *new_cache = NULL;
#ifdef __cplusplus
        new_cache = new seq_t[new_size];
#else
        new_cache = (seq_t*)calloc(new_size, sizeof(seq_t));
#endif
        if (!new_cache) return 0;          /* allocation failed, return 0 */

        /* Copy previously computed values */
        for (size_t i = 0; i < old_size && i < new_size; ++i)
            new_cache[i] = cache[i];

#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
        cache = new_cache;
        cache_size = new_size;

        /* Compute new entries (if any) */
        for (size_t i = old_size; i < cache_size; ++i)
        {
            if (i == 0)         cache[i] = 28;
            else if (i == 1)    cache[i] = 785;
            else if (i == 2)    cache[i] = 43988;
            else if (i == 3)    cache[i] = 1232449;
            else                cache[i] = 1570 * cache[i-2] - cache[i-4];
        }
    }

    return cache[n];
}
