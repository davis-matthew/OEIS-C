/*
 * A182248  a(0)=0, a(n) = (a(n-1) OR n) + n.
 * https://oeis.org/A182248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (a(n-1) OR n) + n; a(0) = 0
 * Range:     u8 n <= 17, u16 n <= 297, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 8114 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    /* initialise cache for n = 0 */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = mem_new(seq_t, cache_cap);
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    /* grow capacity if necessary */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap;
        while ((idx_t)new_cap <= n) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t val  = (prev | (seq_t)i) + (seq_t)i;
        cache[i] = val;
    }
    cache_len = n + 1;

    return cache[n];
}
