/*
 * A052143  E.g.f.: exp(x)/sqrt(1-4*x).
 * https://oeis.org/A052143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n-1)*a(n-1) - 4*(n-1)*a(n-2) with initial values
 *            a(0)=1, a(1)=3. This recurrence follows from the EGF of the
 *            sequence
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n) for OEIS A052143 */
seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    idx_t i;

    /* initialise cache for n==0 or n==1 */
    if (cache_size == 0) {
        cache_size = 2;                      /* at least one slot for a(0) and a(1) */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
    }

    /* extend cache up to requested index */
    if (n >= cache_size) {
        /* new size is at least n+1, grow geometrically for efficiency */
        idx_t new_size = n + 1;
        if (new_size < 2) new_size = 2;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        /* compute missing terms using the recurrence */
        for (i = cache_size; i <= n; ++i) {
            seq_t coef1 = (seq_t)(4 * i - 1);          /* (4*i-1) as seq_t */
            seq_t term1 = coef1 * cache[i - 1];        /* (4*i-1)*a(i-1) */
            seq_t coef2 = (seq_t)(4 * (i - 1));        /* 4*(i-1) as seq_t */
            seq_t term2 = coef2 * cache[i - 2];        /* 4*(i-1)*a(i-2) */
            cache[i] = term1 - term2;                 /* recurrence */
        }
        cache_size = new_size;
    }

    return cache[n];
}
