/*
 * A137531  a(n) = 3*a(n-1) - 2*a(n-2) + a(n-3).
 * https://oeis.org/A137531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 51, u128 n <= 104,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 953 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=4, a(2)=10,
   a(n)=3*a(n-1)-2*a(n-2)+a(n-3) for n≥3 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Base cases for small n */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)10;

    /* Initialise cache with first 3 values if needed */
    if (cache == NULL) {
        cache_size = 3;
        cache = mem_new(seq_t, cache_size);
        if (cache == NULL) return 0;          /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)10;
    }

    /* Extend cache up to requested n */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = mem_resize(seq_t, cache, new_size);
        if (new_cache == NULL) return 0;      /* allocation failed */
        cache = new_cache;
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            cache[i] = 3 * cache[i-1] - 2 * cache[i-2] + cache[i-3];
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
