/*
 * A206087  Number of (n+1) X 2 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly two counterclockwise and two clockwise edge
 *          increases.
 * https://oeis.org/A206087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - a(n-2) + 12*a(n-3) - 36*a(n-4) Initial values:
 *            a(1)=81, a(2)=270, a(3)=972, a(4)=3564 Compute by recurrence,
 *            memoizing previous terms for arbitrary call order
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* static cache of computed values, 1‑based index */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* highest index computed */

    /* initialize the cache on first call */
    if (cache == NULL)
    {
        cache_cap = 5;  /* array indices 0..4, we use 1..4 */
        cache = seq_alloc_new_array(cache_cap);
        /* initialise the first four terms (index 0 unused) */
        cache[1] = 81;
        cache[2] = 270;
        cache[3] = 972;
        cache[4] = 3564;
        cache_len = 4;
    }

    /* if requested term already known, return it */
    if ((size_t)n <= cache_len)
        return cache[n];

    /* enlarge the cache if necessary */
    if ((size_t)n + 1 > cache_cap)
    {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap) new_cap *= 2;
        seq_t *new_cache = seq_alloc_new_array(new_cap);
        /* copy old values */
        for (size_t i = 1; i <= cache_len; ++i)
            new_cache[i] = cache[i];
        seq_free_array(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        /* recurrence: a_i = 4*a_{i-1} - a_{i-2} + 12*a_{i-3} - 36*a_{i-4} */
        seq_t t1 = 4 * cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = 12 * cache[i - 3];
        seq_t t4 = 36 * cache[i - 4];
        cache[i] = t1 - t2 + t3 - t4;
    }
    cache_len = n;  /* update length */

    return cache[n];
}
