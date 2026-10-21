/*
 * A227040  Positive solutions y/5^3 of the Pell equation x^2 - 73*y^2 = -1.
 * https://oeis.org/A227040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4562498*a(n-1) - a(n-2), with a(-1) = 1, a(0) = 1
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the recurrence for A227040, storing computed values in a static cache. */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements currently stored (max index + 1) */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* initialise cache with a(0) if necessary */
    if (cache_len == 0) {
        cache_cap = 16;           /* start with a modest capacity */
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out of memory guard, unlikely in tests */
        cache[0] = (seq_t)1;      /* a(0) = 1 */
        cache_len = 1;
    }

    /* grow cache if needed and compute missing terms */
    while (cache_len <= n) {
        seq_t prev1 = cache[cache_len - 1];          /* a(i-1) */
        seq_t prev2 = (cache_len >= 2) ? cache[cache_len - 2] : (seq_t)1; /* a(i-2), a(-1)=1 */
        seq_t prod = ((seq_t)4562498) * prev1;       /* multiplication by constant */
        cache[cache_len] = prod - prev2;             /* a(i) = 4562498*a(i-1) - a(i-2) */

        /* resize cache if necessary */
        if (cache_len == cache_cap) {
            cache_cap <<= 1;                         /* double the capacity */
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;              /* out of memory guard */
            cache = tmp;
        }
        cache_len++;
    }

    return cache[n];
}
