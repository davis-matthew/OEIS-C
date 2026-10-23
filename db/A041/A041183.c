/*
 * A041183  Denominators of continued fraction convergents to sqrt(102).
 * https://oeis.org/A041183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 202*a(n-2) - a(n-4) with initial values a(0) = 1, a(1) =
 *            10, a(2) = 201, a(3) = 2020
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;    /* array of a(0) ... a(k)   */
    static idx_t cache_size = 0;   /* current size of cache    */

    /* handle first four terms directly */
    if (n < 4) {
        switch ((int)n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)10;
            case 2: return (seq_t)201;
            case 3: return (seq_t)2020;
            default: break;
        }
    }

    /* if requested index is outside current cache, grow it    */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t*)malloc(sizeof(seq_t) * new_size);
        if (!new_cache) /* out of memory; return 0 as sentinel */
            return (seq_t)0;

        /* copy existing data */
        if (cache) {
            for (idx_t i = 0; i < cache_size; ++i)
                new_cache[i] = cache[i];
        }

        /* ensure the first four values are in place */
        if (cache_size < 4) {
            new_cache[0] = (seq_t)1;
            new_cache[1] = (seq_t)10;
            new_cache[2] = (seq_t)201;
            new_cache[3] = (seq_t)2020;
        }

        /* compute new terms using the recurrence */
        for (idx_t i = (cache_size > 4 ? cache_size : 4); i <= n; ++i) {
            new_cache[i] = (seq_t)202 * new_cache[i-2] - new_cache[i-4];
        }

        /* free old cache and update pointers */
        free(cache);
        cache     = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
