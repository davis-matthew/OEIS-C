/*
 * A137889  Number of directed Hamiltonian paths in the n-Hanoi graph.
 * https://oeis.org/A137889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*a(n-1) - 147*a(n-2) + 316*a(n-3) - 192*a(n-4) for n >
 *            4 with initial values a(1) = 6, a(2) = 36, a(3) = 384, a(4) =
 *            5460
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache of previously computed terms, indexed from 1 */
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;  /* largest index stored in cache */

    /* initialise cache with the first four values if it does not exist yet */
    if (cache == NULL) {
        cache_size = 4;
        /* allocate enough space for indices 0..4 (index 0 unused) */
        cache = (seq_t *)malloc((cache_size + 1) * sizeof(seq_t));
        cache[0] = (seq_t)0;              /* unused */
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)36;
        cache[3] = (seq_t)384;
        cache[4] = (seq_t)5460;
    }

    /* if the requested term is already in the cache, just return it */
    if (n <= cache_size) {
        return cache[n];
    }

    /* extend the cache up to the requested index using the recurrence */
    if (n > cache_size) {
        /* allocate space for indices 0..n */
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failed – abort to avoid undefined behaviour */
            exit(1);
        }
        cache = tmp;

        /* compute missing terms */
        for (idx_t k = cache_size + 1; k <= n; ++k) {
            seq_t term = (seq_t)24 * cache[k - 1];
            term -= (seq_t)147 * cache[k - 2];
            term += (seq_t)316 * cache[k - 3];
            term -= (seq_t)192 * cache[k - 4];
            cache[k] = term;
        }
        cache_size = n;
    }

    return cache[n];
}
