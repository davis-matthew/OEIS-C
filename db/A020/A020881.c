/*
 * A020881  Number of strong restricted edge-subgraphs in Moebius ladder M_n.
 * https://oeis.org/A020881
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2*a(n-1) + 11*a(n-2) + 4*a(n-3) - 4*a(n-4) for n>5
 *            Initial values: a(2)=18, a(3)=102, a(4)=418, a(5)=2006
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;                        /* behavior below the offset is irrelevant */

    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Ensure the cache is large enough to hold all indices up to n */
    idx_t required = n + 1;                     /* indices 0..n inclusive */
    if (required < 6) required = 6;             /* need room for base values 2..5 */
    if (required > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (!new_cache) return 0;               /* allocation failed: return 0 */
        /* Initialize newly allocated slots to zero (sentinel) */
        for (idx_t i = cache_size; i < required; ++i)
            new_cache[i] = 0;
        cache = new_cache;
        cache_size = required;
    }

    /* If the value has already been computed, return it */
    if (cache[n] != 0) return cache[n];

    /* Ensure base values are stored once */
    if (cache[2] == 0) cache[2] = 18;
    if (cache[3] == 0) cache[3] = 102;
    if (cache[4] == 0) cache[4] = 418;
    if (cache[5] == 0) cache[5] = 2006;

    /* Compute values from 6 up to n using the recurrence */
    for (idx_t k = 6; k <= n; ++k) {
        seq_t val = 2 * cache[k - 1] + 11 * cache[k - 2]
                  + 4 * cache[k - 3] - 4 * cache[k - 4];
        cache[k] = val;
    }

    return cache[n];
}
