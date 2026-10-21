/*
 * A228555  y-values in the solution to the Pell equation x^2 - 85*y^2 = -1.
 * https://oeis.org/A228555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 571538*a(n-1) - a(n-2); a(1) = 41; a(2) = 23433017
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;  /* Outside the valid range, return 0 */
    
    static seq_t *cache = NULL;      /* cache[i] stores a(i+1) */
    static size_t cap = 0;           /* current size of the cache */
    
    /* Ensure we have at least two cached terms */
    if (cache == NULL) {
        cap = 2;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed */
        cache[0] = (seq_t)41;          /* a(1) */
        cache[1] = (seq_t)23433017;    /* a(2) */
    }
    
    /* Grow the cache if necessary */
    if ((size_t)n > cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed */
        cache = tmp;
        /* Compute missing terms */
        for (size_t i = cap; i < new_cap; ++i) {
            /* cache[i-1] = a((i-1)+1) = a(i), cache[i-2] = a(i-1) */
            seq_t term = cache[i-1] * (seq_t)571538;
            term -= cache[i-2];
            cache[i] = term;
        }
        cap = new_cap;
    }
    
    return cache[n-1];   /* cache[0] holds a(1) */
}
