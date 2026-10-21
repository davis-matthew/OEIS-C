/*
 * A084262  Binomial transform of double factorials.
 * https://oeis.org/A084262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A084262 – Binomial transform of double factorials
    a(n) = Sum_{k=0..n} C(n,k) * (2*k)! / (k! * 2^k)
    Recurrence (derived from the OEIS entry):
       a(0) = 1,  a(1) = 2
       a(n) = 2*n*a(n-1) - 2*(n-1)*a(n-2)   (n >= 2)
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache – grows as large n values are requested. */
    static seq_t *cache = NULL;   /* array of computed a(i) */
    static idx_t  cache_len = 0;  /* number of pre‑computed terms */

    /* Initialise the cache with the first two values when first called */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)1; /* a(0) */
        cache[1] = (seq_t)2; /* a(1) */
    }

    /* If the requested index is already cached, return it immediately */
    if (n < cache_len)
        return cache[n];

    /* Grow the cache to accommodate the desired index */
    idx_t new_len = n + 1;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (cache == NULL) {
        /* allocation failed – return 0 as a safe default (should not happen) */
        return (seq_t)0;
    }

    /* Compute all missing terms up to n using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)2;
        } else {
            /* term1 = 2 * i * a(i-1) */
            seq_t term1 = ((seq_t)2 * (seq_t)i) * cache[i - 1];
            /* term2 = 2 * (i-1) * a(i-2) */
            seq_t term2 = ((seq_t)2 * (seq_t)(i - 1)) * cache[i - 2];
            cache[i] = term1 - term2;
        }
    }

    /* Update stored length and return the requested value */
    cache_len = new_len;
    return cache[n];
}
