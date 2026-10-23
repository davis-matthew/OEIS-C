/*
 * A106805  Expansion of g.f.: 1/(1 - 2*x - x^2 + x^3).
 * https://oeis.org/A106805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 54, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A106805: Expansion of g.f. 1/(1 - 2*x - x^2 + x^3)
 * Recurrence: a(0)=1, a(1)=2, a(2)=5,
 *             a(n) = 2*a(n-1) + a(n-2) - a(n-3)  (n >= 3)
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;      /* array of computed terms */
    static idx_t  last_computed = 0; /* highest index computed so far */
    static size_t cache_cap = 0;     /* allocated capacity */

    /* Initial base values (ensure array has at least 3 slots) */
    if (cache_cap == 0) {
        cache_cap = 4; /* small initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure fallback */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)5;
        last_computed = 2;
    }

    /* Allocate more space if needed */
    if ((size_t)n > last_computed && (size_t)n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure fallback */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n */
    while (last_computed < n) {
        ++last_computed; /* compute next term */
        idx_t i = last_computed;
        /* recurrence: a(i) = 2*a(i-1) + a(i-2) - a(i-3) */
        cache[i] = cache[i-1] * (seq_t)2 + cache[i-2] - cache[i-3];
    }

    return cache[n];
}
