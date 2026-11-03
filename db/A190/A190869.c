/*
 * A190869  a(n) = 10*a(n-1) - 2*a(n-2), a(0)=0, a(1)=1.
 * https://oeis.org/A190869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 2*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 807 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* handle base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with two elements if first call */
    if (!cache) {
        /* allocate space for n = 0 and n = 1 */
        cache = mem_new(seq_t, 2);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_size = 2;
    }

    /* expand cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = mem_resize(seq_t, cache, new_size);
        if (!new_cache) { /* allocation failed – fall back to manual calculation */
            /* simple linear evaluation without caching */
            seq_t a0 = 0, a1 = 1, an;
            for (idx_t i = 2; i <= n; ++i) {
                an = a1 * (seq_t)10 - a0 * (seq_t)2;
                a0 = a1;
                a1 = an;
            }
            return an;
        }
        cache = new_cache;
        /* compute missing terms iteratively */
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = cache[i-1] * (seq_t)10 - cache[i-2] * (seq_t)2;
        }
        cache_size = new_size;
    }

    return cache[n];
}
