/*
 * A033655  Trajectory of 19 under map x->x + (x-with-digits-reversed).
 * https://oeis.org/A033655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 19, a(n) = a(n−1) + rev_decimal(a(n−1))
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/rev_decimal.h"

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;     /* array of previously computed terms */
    static idx_t  cache_len = 0;    /* number of terms stored (next free index) */
    static size_t cache_cap = 0;    /* allocated capacity of the array */

    /* If the requested term is already cached, return it immediately */
    if (n < cache_len) {
        return cache[n];
    }

    /* Initialise the cache if this is the first call */
    if (!cache) {
        /* Allocate at least enough space for the required term */
        cache_cap = (size_t)(n + 1);
        if (cache_cap < 16) {
            cache_cap = 16;                    /* small default size */
        }
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;            /* unlikely, but safe */

        /* Base term of the sequence */
        cache[0] = (seq_t)19;
        cache_len = 1;
    }

    /* Grow the cache if it cannot hold the new term */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)(n + 1)) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;              /* unlikely, but safe */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute all terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t rev  = rev_decimal(prev);
        cache[i] = prev + rev;
    }

    cache_len = n + 1;
    return cache[n];
}
