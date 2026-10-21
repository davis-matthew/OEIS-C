/*
 * A203536  Number of nX2 0..2 arrays with every element neighboring
 *          horizontally or vertically both a 0 and a 1.
 * https://oeis.org/A203536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*a(n-1) + 3*a(n-2) + 6*a(n-3) - a(n-4) - a(n-6),
 * a(1) = 0, a(2) = 4, a(3) = 4, a(4) = 16, a(5) = 64, a(6) = 196
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoization cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Ensure cache is large enough and initialize base cases when needed */
    if (cache_size == 0) {
        /* Allocate space for the first seven valid indices (0..6) */
        cache = (seq_t *)malloc(7 * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* Out‑of‑memory: return 0 (unlikely) */
        cache[0] = (seq_t)0;          /* unused, placeholder */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)16;
        cache[5] = (seq_t)64;
        cache[6] = (seq_t)196;
        cache_size = 7;
    }

    /* If the requested index is beyond the current cache, extend it */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* Out‑of‑memory: return 0 (unlikely) */
        cache = new_cache;
        /* Compute missing terms up to index n */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i <= 6) {
                /* Base cases already initialized; skip recomputation */
                continue;
            }
            /* Recurrence: a(i) = 2*a(i-1) + 3*a(i-2) + 6*a(i-3) - a(i-4) - a(i-6) */
            cache[i] = ((seq_t)2 * cache[i-1]) +
                       ((seq_t)3 * cache[i-2]) +
                       ((seq_t)6 * cache[i-3]) -
                       cache[i-4] -
                       cache[i-6];
        }
        cache_size = new_size;
    }

    return cache[n];
}
