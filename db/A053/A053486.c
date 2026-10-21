/*
 * A053486  E.g.f.: exp(3x)/(1-x).
 * https://oeis.org/A053486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   cast_ptr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A053486
    Formula: a(n) = \sum_{k=0}^{n} \frac{n!}{k!}\,3^k
    Recurrence (using only integer arithmetic):
       a(0) = 1
       a(1) = 4
       a(n) = (n+3)·a(n-1) – 3·(n-1)·a(n‑2)    for n ≥ 2
    The implementation accumulates terms in a static cache so that
    calls in arbitrary order are handled in O(n) time per call.
*/

#include <stdlib.h>
#include "../utils/cast_ptr.h"

seq_t a(idx_t n)
{
    /* static cache: cache[0] = a(0), cache[1] = a(1) */
    static seq_t *cache          = NULL;
    static idx_t cache_capacity  = 0;   /* size of allocated array  */
    static idx_t max_n_computed  = (idx_t)(-1); /* largest index stored */

    /* initialize cache on first call */
    if (cache == NULL) {
        cache_capacity = 2;                                /* need indices 0 and 1 */
        cache = cast_ptr( calloc( cache_capacity, sizeof(seq_t) ) );
        if (!cache) return (seq_t)0;                       /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        max_n_computed = 1;
    }

    /* if the value has already been computed, return it */
    if (n <= max_n_computed)
        return cache[n];

    /* extend the cache so that it can hold indices up to n */
    if (n + 1 > cache_capacity) {
        idx_t new_capacity = cache_capacity;
        while (new_capacity <= n)
            new_capacity <<= 1;                           /* double until big enough */
        seq_t *tmp = cast_ptr( realloc( cache, new_capacity * sizeof(seq_t) ) );
        if (!tmp) return (seq_t)0;                       /* allocation failure */
        cache          = tmp;
        cache_capacity = new_capacity;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = max_n_computed + 1; i <= n; ++i) {
        seq_t term1 = ((seq_t)(i + 3)) * cache[i - 1];
        seq_t term2 = ((seq_t)(3 * (i - 1))) * cache[i - 2];
        cache[i] = term1 - term2;
    }
    max_n_computed = n;
    return cache[n];
}
