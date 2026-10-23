/*
 * A017818  Expansion of 1/(1-x^3-x^4-x^5).
 * https://oeis.org/A017818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-5) + a(n-4) + a(n-3) for n ≥ 5; initial values:
 *            a(0)=1, a(1)=0, a(2)=0, a(3)=1, a(4)=1
 * Range:     u8 n <= 24, u16 n <= 44, u32 n <= 83, u64 n <= 162,
 *            u128 n <= 320, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* for size_t */
#include <stdlib.h>     /* for malloc, free */

#include "../utils/seq_alloc.h"     /* seq_alloc(size_t) */
#include "../utils/seq_realloc.h"  /* seq_realloc(seq_t*, size_t, size_t) */

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;     /* pointer to array of seq_t */
    static size_t cache_cap = 0;    /* capacity of the array */
    static idx_t next_missing = 0;  /* first index that has not been computed */

    /* Ensure the cache can hold index n */
    if ((size_t)n + 1 > cache_cap) {
        seq_t *new_cache;
        size_t new_cap = (size_t)n + 1;

        if (cache == NULL) {
            new_cache = seq_alloc(new_cap);
        } else {
            new_cache = seq_realloc(cache, cache_cap, new_cap);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute all values from next_missing up to n inclusive */
    for (idx_t i = next_missing; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1 || i == 2) {
            cache[i] = (seq_t)0;
        } else if (i == 3 || i == 4) {
            cache[i] = (seq_t)1;
        } else {
            /* recurrence: a(i) = a(i-5) + a(i-4) + a(i-3) */
            cache[i] = cache[i-5] + cache[i-4] + cache[i-3];
        }
    }

    /* Update state: next index to compute */
    next_missing = n + 1;

    return cache[n];
}
