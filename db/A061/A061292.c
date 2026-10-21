/*
 * A061292  a(n) = a(n-1)*a(n-2)*a(n-3) - a(n-4) for n>3 with a(0) = a(1) =
 *          a(2) = a(3) = 2.
 * https://oeis.org/A061292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = a(n-1)*a(n-2)*a(n-3) - a(n-4) with a(0) = a(1) = a(2) =
 *            a(3) = 2
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 8, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of values currently stored (cache_size == max_index+1) */
    static idx_t cache_cap  = 0;   /* allocated capacity (number of slots) */

    /* Special case: indices < 4 */
    if (n < 4)
        return (seq_t)2;

    /* Allocate or grow the cache if necessary */
    if (cache_cap == 0) {
        cache_cap = 8;                               /* start with a small capacity */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }
    if (n >= cache_cap) {                            /* ensure space for index n */
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;          /* double until we fit */
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing entries up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 4)
            cache[i] = (seq_t)2;
        else
            cache[i] = cache[i-1] * cache[i-2] * cache[i-3] - cache[i-4];
    }
    cache_size = n + 1;   /* we now have values up to index n */

    return cache[n];
}
