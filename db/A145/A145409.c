/*
 * A145409  Number of perfect matchings (or domino tilings) in K_6 X P_n.
 * https://oeis.org/A145409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 21*a(n-1) + 62*a(n-2) - 21*a(n-3) - a(n-4) with initial
 *            values a(1)=15, a(2)=376, a(3)=8805, a(4)=207901
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle indices below the offset (undefined behaviour for this
       problem; we simply return 0). */
    if (n == 0) return 0;

    /* Initial values for the first four terms. */
    static const seq_t init[4] = { (seq_t)15, (seq_t)376,
                                   (seq_t)8805, (seq_t)207901 };

    /* If the call asks for one of the seed terms, return it immediately. */
    if (n <= 4) return init[n - 1];

    /* Static cache to store already computed values. */
    static seq_t *cache = NULL;
    static idx_t size = 0;          /* first 'size' elements are valid */
    static idx_t cap   = 0;          /* allocated capacity */

    /* Allocate or grow the cache as necessary. */
    if (cache == NULL) {
        /* Start with a small capacity, there are at least four terms. */
        cap = 16;
        if (cap < 4) cap = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        for (idx_t i = 0; i < 4; ++i) cache[i] = init[i];
        size = 4;
    } else if (cap < n) {
        idx_t new_cap = cap * 2;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) {
            /* Allocation failed: give up (undefined for the caller). */
            return 0;
        }
        cache = tmp;
        cap = new_cap;
    }

    /* Extend the cached values up to the requested term. */
    for (idx_t i = size; i < n; ++i) {
        seq_t val = (seq_t)21 * cache[i - 1] +
                    (seq_t)62 * cache[i - 2] -
                    (seq_t)21 * cache[i - 3] -
                    cache[i - 4];
        cache[i] = val;
    }
    size = n;

    return cache[n - 1];
}
