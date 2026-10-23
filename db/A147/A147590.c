/*
 * A147590  Numbers whose binary representation is the concatenation of 2n-1
 *          digits 1 and n-1 digits 0.
 * https://oeis.org/A147590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 10*a(n-1) - 16*a(n-2), a(1)=1, a(2)=14
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n<1, return 0 */

    static seq_t *cache = NULL;           /* cache of computed terms */
    static idx_t cached = 0;             /* number of entries in cache */

    /* if we already have the value, return it */
    if (n <= cached) return cache[n-1];

    /* expand the cache to size n */
    idx_t new_size = n;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();                    /* allocation failure */
    cache = tmp;

    /* initialise first two terms if they were not already set */
    if (cached == 0) {
        cache[0] = (seq_t)1;              /* a(1) */
    }
    if (cached < 2 && new_size >= 2) {
        cache[1] = (seq_t)14;             /* a(2) */
    }

    /* adjust cached to at least 2 so the recurrence can start */
    if (cached < 2) cached = 2;

    /* compute all missing terms up to n using recurrence */
    for (idx_t i = cached; i < n; ++i) {
        cache[i] = (seq_t)10 * cache[i-1] - (seq_t)16 * cache[i-2];
    }

    cached = n;
    return cache[n-1];
}
