/*
 * A193437  Expansion of e.g.f. exp( Sum_{n>=0} x^(3*n+1)/(3*n+1) ).
 * https://oeis.org/A193437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0)=a(1)=a(2)=1,
   a(n)=a(n-1)+(n-3)*(n-2)*(n-1)*a(n-3)  for n≥3   */
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;          /* size of allocated array   */
    static idx_t last_computed = (idx_t)-1;   /* greatest index set in cache */

    /* enlarge cache until it can hold index n */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap > 0 ? cache_cap : 1;
        while (new_cap <= (idx_t)n) new_cap <<= 1;   /* double until enough */

        seq_t *new_cache = alloc_seq_array((size_t)new_cap);
        /* copy old values */
        for (idx_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
        /* free old array */
        if (cache != NULL) free_seq_array(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* initialise base values lazily */
    if (last_computed < 2) {
        cache[0] = (seq_t)1;
        if (cache_cap > 1) cache[1] = (seq_t)1;
        if (cache_cap > 2) cache[2] = (seq_t)1;
        /* last_computed is the largest index we really set */
        last_computed = cache_cap > 2 ? (idx_t)2 : (cache_cap - 1);
    }

    /* compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i < 3) {              /* should never happen after init */
            cache[i] = (seq_t)1;
        } else {
            seq_t prod = (seq_t)(i - 3) * (seq_t)(i - 2) * (seq_t)(i - 1) * cache[i - 3];
            cache[i] = cache[i - 1] + prod;
        }
    }
    last_computed = n;

    return cache[n];
}
