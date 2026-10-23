/*
 * A015537  Expansion of x/(1 - 5*x - 4*x^2).
 * https://oeis.org/A015537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 4*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Ensure cache is initialized for n = 0 and 1 */
    if (n == 0)
        return 0;

    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache)
            return 0;            /* out of memory – unlikely for these limits */
        cache[0] = 0;
        cache[1] = 1;
    }

    if (n < cache_size)
        return cache[n];

    /* Extend cache to hold values up to n */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {
        cache_size = 0;
        free(cache);
        cache = NULL;
        return 0;                /* out of memory */
    }
    cache = tmp;

    /* Compute new terms using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = 5 * cache[i - 1] + 4 * cache[i - 2];
    }
    cache_size = new_size;
    return cache[n];
}
