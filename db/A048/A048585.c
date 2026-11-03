/*
 * A048585  Pisot sequence L(6,7).
 * https://oeis.org/A048585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 26, u32 n <= 55, u64 n <= 113,
 *            u128 n <= 229, big any n
 * Validated: standard, pass, 58 terms through n = 914 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=6, a(1)=7, a(2)=9, a(3)=12,
   a(n)=2*a(n-1)-a(n-2)+a(n-3)-a(n-4) for n≥4 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* allocated size of cache */
    static idx_t max_computed = 3; /* highest index that has been computed */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_size = 4;
        cache = mem_new(seq_t, cache_size);
        if (!cache) return (seq_t)0;     /* allocation failure */
        cache[0] = (seq_t)6;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)12;
    }

    /* grow cache if requested index is beyond current capacity */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = mem_resize(seq_t, cache, new_size);
        if (!new_cache) return (seq_t)0;     /* allocation failure */
        cache = new_cache;
        cache_size = new_size;
    }

    /* compute any missing terms up to n */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* a(i) = 2*a(i-1) - a(i-2) + a(i-3) - a(i-4) */
            seq_t term = (2 * cache[i - 1]) - cache[i - 2]
                         + cache[i - 3] - cache[i - 4];
            cache[i] = term;
        }
        max_computed = n;
    }

    return cache[n];
}
