/*
 * A052545  Expansion of (1-x)^2/(1-3*x+x^3).
 * https://oeis.org/A052545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-3) with a(0)=1, a(1)=1, a(2)=4
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 998 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* trivial cases – no need to bother the cache                     */
    if (n == 0 || n == 1) return (seq_t)1;
    if (n == 2)           return (seq_t)4;

    /* static cache that holds already computed values                  */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t next_compute = 3;  /* next index that still needs to be filled */

    /* initialise cache on the first real call (n ≥ 3)                  */
    if (cache == NULL) {
        cache_size = 3;
        cache = mem_new(seq_t, cache_size);
        if (!cache) return (seq_t)0;           /* allocation failed – give up */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
    }

    /* grow the cache so that index n is within bounds                 */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_size);
        if (!tmp) { /* realloc failed – keep old cache contents */
            tmp = cache;
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* compute any values that have not been filled yet                */
    if (next_compute < cache_size) {
        for (idx_t i = next_compute; i < cache_size; ++i) {
            cache[i] = (seq_t)3 * cache[i-1] - cache[i-3];
        }
        next_compute = cache_size;
    }

    return cache[n];
}
