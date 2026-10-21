/*
 * A055596  Expansion of e.g.f. (2 - x - 2*exp(-x))/(1-x).
 * https://oeis.org/A055596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1) * (a(n-1) + a(n-2)), a(1)=1, a(2)=0
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache[0] is unused, cache[1] = 1, cache[2] = 0 */
    static seq_t *cache = NULL;      /* dynamically allocated array */
    static idx_t cache_len = 0;      /* number of usable indices (size of array) */

    /* Ensure there is room for index 2 at minimum */
    if (cache_len < 3) {
        /* Allocate indices 0..2 */
        idx_t new_len = 3;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)0;
        cache_len = new_len;
    }

    /* Extend cache if n is beyond current capacity */
    if ((idx_t)n >= cache_len) {
        idx_t new_len = n + 1;  /* indices 0..n inclusive */
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache_len = new_len;
    }

    /* Compute missing terms up to n */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t sum = cache[i - 1] + cache[i - 2];
        cache[i] = ((seq_t)(i - 1)) * sum;
    }

    return cache[n];
}
