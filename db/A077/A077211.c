/*
 * A077211  a(1) = 1; a(n+1) is the smallest square greater than n*a(n).
 * https://oeis.org/A077211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n+1) = (floor(sqrt(n * a(n))) + 1)²
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes any term a(n) for n ≥ 1, memoizing results in a static array. */

#include <stdlib.h>
#include "../utils/floor_sqrt.h"

extern seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i) for i ≥ 1.
       cache[0] is unused (set to 0).  cached_size records the highest
       index that has been computed. */
    static seq_t *cache = NULL;
    static idx_t cached_size = 0;

    /* Expand the cache if necessary. */
    if (cached_size < n) {
        size_t new_cap = (size_t)n + 1;   /* capacity for indices 0..n */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (cached_size == 0) {
            /* Initialise the first term. */
            cache[0] = (seq_t)0;
            cache[1] = (seq_t)1;
            cached_size = 1;
        }
        for (idx_t i = cached_size + 1; i <= n; ++i) {
            /* For a(i), use n = i‑1 in the recurrence. */
            seq_t prod = ((seq_t)(i - 1)) * cache[i - 1];      /* (i-1) * a(i-1) */
            seq_t floor_sq = floor_sqrt(prod);                /* floor(sqrt(prod)) */
            seq_t base = floor_sq + (seq_t)1;                 /* floor + 1 */
            cache[i] = base * base;                          /* next square */
        }
        cached_size = n;
    }

    return cache[n];
}
