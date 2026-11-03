/*
 * A170933  a(n) = A000930(n) + A000930(n+3) + 4.
 * https://oeis.org/A170933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) - 4 (a(0)=7, a(1)=8, a(2)=9)
 * Range:     u8 n <= 12, u16 n <= 26, u32 n <= 55, u64 n <= 113,
 *            u128 n <= 229, big any n
 * Validated: standard, pass, 51 terms through n = 825 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache of computed terms; grows as needed */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Initial allocation and base values */
    if (cache == NULL) {
        cache_size = 3;
        cache = mem_new(seq_t, cache_size);
        if (cache == NULL) return (seq_t)0;       /* out of memory: return 0 */
        cache[0] = (seq_t)7;
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)9;
    }

    /* Extend cache if necessary */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = mem_resize(seq_t, cache, new_size);
        if (new_cache == NULL) return (seq_t)0;   /* out of memory: return 0 */
        cache = new_cache;
        /* Compute new terms using the recurrence */
        for (size_t i = cache_size; i <= n; ++i) {
            cache[i] = cache[i - 1] + cache[i - 3] - (seq_t)4;
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
