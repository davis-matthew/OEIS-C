/*
 * A094705  Convolution of Jacobsthal(n) and 3^n.
 * https://oeis.org/A094705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - a(n-2) - 6*a(n-3), with a(0)=0, a(1)=1, a(2)=4
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to allow out‑of‑order calls and avoid recomputation. */
    static seq_t *cache = NULL;          /* array holding a(0) .. a(max_cached-1) */
    static idx_t  cache_size = 0;        /* number of stored terms */

    /* If requested term already cached, return it immediately. */
    if (n < cache_size)
        return cache[n];

    /* Grow the cache to hold terms up to n. */
    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) {            /* allocation failed – return 0 as fallback */
        return 0;
    }
    cache = new_cache;

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t value;
        if (i == 0)          value = 0;
        else if (i == 1)      value = 1;
        else if (i == 2)      value = 4;
        else {
            value = 4 * cache[i - 1];
            value = value - cache[i - 2];
            value = value - 6 * cache[i - 3];
        }
        cache[i] = value;
    }

    cache_size = new_size;
    return cache[n];
}
