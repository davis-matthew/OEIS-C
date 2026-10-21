/*
 * A054885  Number of walks of length n along the edges of an icosahedron
 *          between two opposite vertices.
 * https://oeis.org/A054885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = 4*a(n-1) + 10*a(n-2) – 20*a(n-3) – 25*a(n-4), a(0) = 0,
 *            a(1) = 0, a(2) = 0, a(3) = 10
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache      = NULL;      /* array of terms          */
    static idx_t  cache_size = 0;         /* number of stored terms  */
    static size_t  cache_cap  = 0;         /* allocated capacity      */

    /* Initialise the first four terms */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)10;
        cache_size = 4;
    }

    /* Return directly if already known */
    if (n < cache_size) {
        return cache[n];
    }

    /* Grow the cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) {
            new_cap <<= 1;          /* double the capacity */
        }
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed – fall back to 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms iteratively */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t t1 = cache[i-1] * (seq_t)4;
        seq_t t2 = cache[i-2] * (seq_t)10;
        seq_t t3 = cache[i-3] * (seq_t)20;
        seq_t t4 = cache[i-4] * (seq_t)25;
        seq_t sum = t1 + t2;
        cache[i] = sum - t3 - t4;     /* the recurrence guarantees non‑negative result */
    }

    cache_size = n + 1;
    return cache[n];
}
