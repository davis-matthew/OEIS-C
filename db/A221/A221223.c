/*
 * A221223  Number of n X 2 arrays of occupancy after each element stays put
 *          or moves to some king-move neighbor.
 * https://oeis.org/A221223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) - 43*a(n-2) + 10*a(n-3), a(1)=3, a(2)=35,
 *            a(3)=400
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i) for i <= cache_cap */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1 */

    /* initialize base values once */
    if (cache_cap == 0) {
        cache_cap = 3;  /* we have values up to a(3) */
        cache = (seq_t*)malloc(sizeof(seq_t) * (cache_cap + 1)); /* +1 for index 0 */
        if (!cache) abort();          /* allocation failure */
        cache[0] = (seq_t)0;          /* unused slot */
        cache[1] = (seq_t)3;          /* a(1) */
        cache[2] = (seq_t)35;         /* a(2) */
        cache[3] = (seq_t)400;        /* a(3) */
    }

    /* if already computed, return it */
    if ((size_t)n <= cache_cap)
        return cache[n];

    /* we need to compute up to n */
    size_t old_cap = cache_cap;
    size_t new_cap = (size_t)n;
    seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * (new_cap + 1));
    if (!new_cache) abort();          /* allocation failure */
    cache = new_cache;
    cache_cap = new_cap;

    /* fill in missing terms using the recurrence */
    for (idx_t i = old_cap + 1; i <= n; ++i) {
        cache[i] =
            (seq_t)15 * cache[i-1] -
            (seq_t)43 * cache[i-2] +
            (seq_t)10 * cache[i-3];
    }

    return cache[n];
}
