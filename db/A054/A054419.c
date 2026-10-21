/*
 * A054419  Number of connected 3 X n binary matrices (divided by 2).
 * https://oeis.org/A054419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A054419 – Number of connected 3×n binary matrices (divided by 2)
    Formula:  a(n) = 7*a(n-1) - 3*a(n-2) + 5*a(n-3)   for n ≥ 4
    Initial values:  a(1)=3, a(2)=21, a(3)=143                         */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache   = NULL;
    static idx_t cache_size   = 0;   /* allocated array size (including index 0) */
    static idx_t computed_max = 0;   /* largest index whose value is stored */

    /* a(n) is defined only for n ≥ 1 */
    if (n < 1)
        return (seq_t)0;

    /* First call: allocate at least 4 slots (indices 0..3) */
    if (cache == NULL) {
        /* If the requested n is small, still make room for the first 3 terms */
        cache_size = (n >= 4 ? n + 1 : 4);
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        /* initialise the base cases */
        cache[0] = (seq_t)0;   /* unused */
        cache[1] = (seq_t)3;   /* a(1) */
        cache[2] = (seq_t)21;  /* a(2) */
        cache[3] = (seq_t)143; /* a(3) */
        computed_max = 3;
    }

    /* Grow the array if we are asked for a larger index */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache_size = new_size;
    }

    /* Compute any missing terms up to n */
    if (n > computed_max) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            seq_t val = cache[i - 1] * (seq_t)7;
            val -= cache[i - 2] * (seq_t)3;
            val += cache[i - 3] * (seq_t)5;
            cache[i] = val;
        }
        computed_max = n;
    }

    return cache[n];
}
