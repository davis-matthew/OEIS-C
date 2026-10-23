/*
 * A024114  Expansion of g.f. 1/((1-x)*(1-6*x)*(1-8*x)*(1-12*x)).
 * https://oeis.org/A024114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(0)=1, a(1)=27, a(2)=487, a(3)=7407;
   a(n)=27*a(n-1)-242*a(n-2)+792*a(n-3)-576*a(n-4)  for n≥4
   This sequence is the expansion of 1/((1-x)(1-6x)(1-8x)(1-12x)). */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of values currently stored */
    static size_t cache_cap = 0;   /* allocated capacity (in terms of seq_t elements) */

    /* If the cache already contains the requested value, return it. */
    if (n < (idx_t)cache_len)
        return cache[(size_t)n];

    /* Expand the cache if necessary. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1U : 16U;
        while (new_cap <= (size_t)n)
            new_cap <<= 1U;

        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failure – abort with a sensible value */
            return (seq_t)0;
        }

        /* Copy existing entries. */
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];

        /* Free old buffer if any. */
        if (cache)
            free(cache);

        cache      = new_cache;
        cache_cap  = new_cap;
        /* cache_len unchanged */
    }

    /* Compute missing terms up to n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i == 0U)
            cache[i] = (seq_t)1;
        else if (i == 1U)
            cache[i] = (seq_t)27;
        else if (i == 2U)
            cache[i] = (seq_t)487;
        else if (i == 3U)
            cache[i] = (seq_t)7407;
        else {
            /* recurrence: 27*a(n-1)-242*a(n-2)+792*a(n-3)-576*a(n-4) */
            seq_t val = 27 * cache[i-1]
                      - 242 * cache[i-2]
                      + 792 * cache[i-3]
                      - 576 * cache[i-4];
            cache[i] = val;
        }
    }

    cache_len = (size_t)n + 1U;
    return cache[(size_t)n];
}
