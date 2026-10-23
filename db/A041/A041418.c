/*
 * A041418  Numerators of continued fraction convergents to sqrt(224).
 * https://oeis.org/A041418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 30*a(n-2) - a(n-4), with initial values * a(0)=14,
 *            a(1)=15, a(2)=434, a(3)=449
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed_max = 0;

    /* Initial allocation and base values */
    if (!cache) {
        idx_t min_size = (n < 4) ? 4 : n + 1;
        cache = (seq_t *)malloc(min_size * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failure */
        cache_size = min_size;
        cache[0] = (seq_t)14;
        cache[1] = (seq_t)15;
        cache[2] = (seq_t)434;
        cache[3] = (seq_t)449;
        computed_max = 3;
    }

    /* Grow cache if needed */
    if ((idx_t)n >= cache_size) {
        idx_t new_size = (idx_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute values up to n */
    if ((idx_t)n > computed_max) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            cache[i] = (seq_t)30 * cache[i - 2] - cache[i - 4];
        }
        computed_max = n;
    }

    return cache[n];
}
