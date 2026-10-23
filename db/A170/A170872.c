/*
 * A170872  a(0) = 1, a(1) = 2; a(2^i + j) = 3a(j) + 3a(j + 1) for 0 <= j <
 *          2^i.
 * https://oeis.org/A170872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   msb_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A170872
   a(0) = 1, a(1) = 2;
   a(2^i + j) = 3*a(j) + 3*a(j+1)   for i ≥ 1, 0 ≤ j < 2^i
   (values of type seq_t, indices of type idx_t) */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/msb_pow2.h"

static seq_t *cache      = NULL;   /* memoised values */
static size_t  cache_cap = 0;      /* current capacity of cache */

/* grow cache to at least new_cap elements */
static void grow_cache(size_t new_cap)
{
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (cache != NULL) {
        for (size_t i = 0; i < cache_cap; ++i) {
            tmp[i] = cache[i];
        }
        free(cache);
    }
    cache      = tmp;
    cache_cap  = new_cap;
}

/* sequence function */
seq_t a(idx_t n)
{
    /* initialise cache for base cases if needed */
    if (cache == NULL) {
        grow_cache(2);           /* minimal size to hold indices 0,1 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
    }

    /* if n is already computed, just return it */
    if (cache_cap > (size_t)n) {
        return cache[(size_t)n];
    }

    /* extend cache to hold indices up to n */
    size_t old_cap = cache_cap;
    grow_cache((size_t)n + 1);

    /* compute new entries in increasing order */
    for (size_t i = old_cap; i <= (size_t)n; ++i) {
        if (i <= 1) continue;           /* base cases already set */
        idx_t p = msb_pow2((idx_t)i);   /* p = 2^i */
        idx_t j = (idx_t)i - p;         /* 0 ≤ j < p */
        seq_t val = cache[(size_t)j];
        val *= (seq_t)3;
        val += cache[(size_t)(j + 1)] * (seq_t)3;
        cache[i] = val;
    }

    return cache[(size_t)n];
}
