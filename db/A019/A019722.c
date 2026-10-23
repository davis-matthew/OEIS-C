/*
 * A019722  Expansion of 1/((1-4x)(1-9x)(1-12x)).
 * https://oeis.org/A019722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*a(n-1) - 192*a(n-2) + 432*a(n-3); a(0) = 1, a(1) = 25,
 *            a(2) = 433
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of allocated entries */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)25;
        cache[2] = (seq_t)433;
    }

    /* If requested term is already in cache, return it */
    if (n < cache_size) {
        return cache[n];
    }

    /* Grow cache to hold up to term n */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
    if (tmp == NULL) {
        /* Allocation failed; return zero to indicate error (shouldn't happen) */
        return (seq_t)0;
    }
    cache = tmp;

    /* Compute new terms using recurrence relation */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val = (seq_t)25 * cache[i - 1];
        val -= (seq_t)192 * cache[i - 2];
        val += (seq_t)432 * cache[i - 3];
        cache[i] = val;
    }

    cache_size = new_size;
    return cache[n];
}
