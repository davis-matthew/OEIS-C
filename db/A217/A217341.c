/*
 * A217341  Number of nX3 arrays of occupancy after each element moves to some
 *          horizontal or vertical neighbor but without 2-loops.
 * https://oeis.org/A217341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 23*a(n-1) -125*a(n-2) +13*a(n-3) +648*a(n-4) *
 *            -208*a(n-5) -1279*a(n-6) +327*a(n-7) +1739*a(n-8) * -1293*a(n-9)
 *            +138*a(n-10) +130*a(n-11) +8*a(n-12); * Initial values
 *            (a(1)..a(12)): * 0, 7, 144, 2389, 37114, 561573, 8399702,
 *            124988061, * 1855434240, 27513859347, 407794083248,
 *            6042695120617
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   mul_seq_by_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_seq_by_int.h"

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* Undefined for n < 1 */

    /* Cached values for n >= 1.  cache[0] unused. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Initial values (a(1) .. a(12)) */
    static const seq_t init_vals[13] = {
        (seq_t)0,
        (seq_t)0,
        (seq_t)7,
        (seq_t)144,
        (seq_t)2389,
        (seq_t)37114,
        (seq_t)561573,
        (seq_t)8399702,
        (seq_t)124988061,
        (seq_t)1855434240,
        (seq_t)27513859347ULL,
        (seq_t)407794083248ULL,
        (seq_t)6042695120617ULL
    };

    /* Coefficients for the recurrence: a(n) = Σ coeff[i-1]*a(n-i), i=1..12 */
    static const int coeff[12] = {
        23,   -125, 13,   648,
        -208, -1279, 327, 1739,
        -1293, 138,  130,  8
    };

    /* Initialise the cache on the very first call. */
    if (cache == NULL) {
        /* Start with capacity for indices 0..12 (i.e. size 13). */
        cache_size = 13;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return (seq_t)0;   /* OOM guard */

        for (size_t i = 0; i < cache_size; ++i) cache[i] = (seq_t)0;
        /* Copy initial known terms. */
        for (size_t i = 1; i < 13; ++i) cache[i] = init_vals[i];
    }

    /* If the requested n is already cached, return it. */
    if ((size_t)n < cache_size) {
        return cache[(size_t)n];
    }

    /* Need to extend the cache to accommodate n. */
    size_t old_size = cache_size;
    size_t new_size = (size_t)n + 1;  /* +1 because index n must fit */

    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
    if (!new_cache) return (seq_t)0;   /* OOM guard */

    cache = new_cache;
    cache_size = new_size;

    /* Compute new entries using the recurrence. */
    for (idx_t i = (idx_t)old_size; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (int k = 0; k < 12; ++k) {
            /* a(i-k-1) corresponds to a(n-(k+1)) */
            seq_t term = cache[(size_t)(i - (k + 1))];
            sum += mul_seq_by_int(term, coeff[k]);
        }
        cache[(size_t)i] = sum;
    }

    return cache[(size_t)n];
}
