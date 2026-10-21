/*
 * A204185  Number of quadrilaterals in a triangular matchstick arrangement of
 *          side n.
 * https://oeis.org/A204185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = a(n-1) + floor((n+1)(n-1)(10n-3)/8); a(0)=0, a(1)=0
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 341, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;

    /* initialise cache for indices 0 and 1 */
    if (cache == NULL) {
        cached_n = 1;
        cache = (seq_t *)malloc((cached_n + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* fallback on allocation failure */
        cache[0] = 0;
        cache[1] = 0;
    }

    /* return cached value if already computed */
    if (n <= cached_n)
        return cache[n];

    /* extend cache up to n and compute new entries */
    idx_t new_n = n;
    seq_t *tmp = (seq_t *)realloc(cache, (new_n + 1) * sizeof(seq_t));
    if (!tmp) return (seq_t)0;              /* fallback on allocation failure */
    cache = tmp;

    for (idx_t i = cached_n + 1; i <= new_n; ++i) {
        seq_t ii = (seq_t)i;
        seq_t term = (ii + 1) * (ii - 1) * (ii * (seq_t)10 - 3);
        seq_t inc  = term / 8;
        cache[i] = cache[i - 1] + inc;
    }

    cached_n = new_n;
    return cache[n];
}
