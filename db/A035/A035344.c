/*
 * A035344  Expansion of 1/((1 - x)*(1 - 4*x + 2 * x^2)).
 * https://oeis.org/A035344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) + 1, a(0)=1, a(1)=5
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Return the n-th term of OEIS A035344 */
seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    /* Initialise the cache on the first call */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
    }

    /* Grow the cache if necessary and compute missing values */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i) {
            /* a(i) = 4*a(i-1) + 1 - 2*a(i-2) */
            cache[i] = (seq_t)4 * cache[i-1] + (seq_t)1
                       - (seq_t)2 * cache[i-2];
        }
        cache_size = new_size;
    }

    return cache[n];
}
