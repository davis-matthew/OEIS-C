/*
 * A189800  a(n) = 6*a(n-1) + 8*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A189800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 8*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;

    /* Allocate initial cache and set base values if needed */
    if (cache == NULL) {
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 0;
        cache[1] = 1;
        max_computed = 1;
    }

    /* Return cached value if already computed */
    if (n <= max_computed) {
        return cache[n];
    }

    /* Extend cache up to the requested index */
    idx_t old_max = max_computed;
    idx_t new_max = n;

    seq_t *new_cache = (seq_t*)realloc(cache, (new_max + 1) * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;

    /* Compute remaining terms using the recurrence */
    for (idx_t k = old_max + 1; k <= new_max; ++k) {
        cache[k] = 6 * cache[k-1] + 8 * cache[k-2];
    }
    max_computed = new_max;

    return cache[n];
}
