/*
 * A085062  a(n) = A085060(n)/9 - 1/3.
 * https://oeis.org/A085062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 2046, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A085062
 * Recurrence: a(0)=0
 *              a(2*n)   = n
 *              a(2*n+1) = 3*a(n)+1
 * The function caches computed values so that repeated calls are O(1) after the
 * first computation of a given index. */

#include <stdlib.h>

/* a : compute the A085062 sequence value */
seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* grow cache if necessary */
    if (n + 1 > cache_size)
    {
        /* new size: allocate at least n+1 entries */
        idx_t new_size = n + 1;
        /* ensure new_size does not overflow */
        if (new_size < cache_size) /* overflow guard */
            return cache[n]; /* return existing if overflow; unlikely */

        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) /* allocation failed: return current value if any */
            return cache[n];
        cache = new_cache;
        cache_size = new_size;
    }

    /* compute values up to n that are not yet known */
    for (idx_t i = 0; i <= n; ++i)
    {
        /* Skip already computed entries */
        /* (the loop starts from 0, but previous values remain unchanged) */
        if (i == 0)
        {
            cache[0] = (seq_t)0;
            continue;
        }
        if (((seq_t)i & 1) == 0) /* even */
        {
            cache[i] = (seq_t)(i >> 1);
        }
        else /* odd: i = 2*k+1 */
        {
            idx_t k = i >> 1;
            /* cache[k] is guaranteed to be computed because k < i */
            cache[i] = (seq_t)3 * cache[k] + (seq_t)1;
        }
    }

    return cache[n];
}
