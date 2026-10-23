/*
 * A102822  a(n+1) is the integer part of sqrt(2*a(n)^2).
 * https://oeis.org/A102822
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 33, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   isqrt, seq_malloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+1) = floor(sqrt(2*a(n)^2)),  a(3) = 3 (offset 3) */
#include <stdlib.h>
#include "../utils/isqrt.h"
#include "../utils/seq_malloc.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialise the cache with the first defined term */
    if (!cache) {
        cache = seq_malloc(4);            /* indices 0 … 3          */
        cache[0] = cache[1] = cache[2] = 0;
        cache[3] = (seq_t)3;
        cache_size = 4;
    }

    /* Extend the cache if the requested index is beyond the current size */
    if (cache_size <= n) {
        seq_t *new_cache = seq_malloc(n + 1);

        /* Copy the already computed values */
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];

        /* Compute terms up to n using the recurrence */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t prev = new_cache[i - 1];
            seq_t val  = (seq_t)2 * prev * prev;  /* 2 * a(i-1)^2   */
            new_cache[i] = isqrt(val);            /* floor(sqrt(val)) */
        }

        free(cache);
        cache = new_cache;
        cache_size = n + 1;
    }

    return cache[n];
}
