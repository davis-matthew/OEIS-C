/*
 * A052598  E.g.f. (1-x)/(1-x-2x^2).
 * https://oeis.org/A052598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=0,
   a(n+2) = (2*n^2+6*n+4)*a(n) + (n+2)*a(n+1)   for n ≥ 0   */
/* This implements OEIS A052598: E.g.f. (1-x)/(1-x-2x^2) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoised values */
    static seq_t *cache  = NULL;     /* array of computed terms   */
    static idx_t cache_size  = 0;    /* number of elements stored */
    static idx_t cache_cap   = 0;    /* current allocated capacity */

    /* Initialise cache with first two terms */
    if (cache_size == 0) {
        cache_cap = 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)0;   /* a(1) */
        cache_size = 2;
    }

    /* Expand capacity if necessary */
    if (cache_cap <= n) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 2;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Return from cache if already computed */
    if (n < cache_size) return cache[n];

    /* Compute forward using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        idx_t m   = i - 2;                     /* m = n in the recurrence a(n+2) */
        seq_t coeff_a = (seq_t)(2 * m * m + 6 * m + 4); /* (2*n^2+6*n+4)  */
        seq_t term1   = coeff_a * cache[m];
        seq_t coeff_b = (seq_t)(m + 2);        /* (n+2)          */
        seq_t term2   = coeff_b * cache[m + 1];
        cache[i] = term1 + term2;
    }
    cache_size = n + 1;
    return cache[n];
}
