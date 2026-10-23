/*
 * A014523  Number of Hamiltonian paths in a 4 X (2n+1) grid starting at the
 *          lower left corner and finishing in the upper right corner.
 * https://oeis.org/A014523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 9*a(n-2) + 7*a(n-3) - a(n-4); a(0) = 1, a(1) =
 *            4, a(2) = 20, a(3) = 111
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Return a cached value if available */
    if (n < cache_size)
        return cache[n];

    /* Ensure the base cases are present */
    if (cache_size == 0) {
        cache_size = 4;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)20;
        cache[3] = (seq_t)111;
    } else if (cache_size < 4) {
        /* Grow to hold the first four terms */
        idx_t new_size = 4;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_size = new_size;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)20;
        cache[3] = (seq_t)111;
    }

    /* Compute the remaining terms up to n */
    idx_t i;
    for (i = cache_size; i <= n; ++i) {
        /* Grow the cache if needed */
        if (i >= cache_size) {
            idx_t new_size = i + 1;
            seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
            for (idx_t j = 0; j < cache_size; ++j)
                new_cache[j] = cache[j];
            free(cache);
            cache = new_cache;
            cache_size = new_size;
        }

        /* The recurrence needs at least i >= 4 */
        seq_t term;
        if (i < 4) {
            /* Base terms already stored */
            term = cache[i];
        } else {
            term = (seq_t)7 * cache[i-1]
                 - (seq_t)9 * cache[i-2]
                 + (seq_t)7 * cache[i-3]
                 - cache[i-4];
        }
        cache[i] = term;
    }

    return cache[n];
}
