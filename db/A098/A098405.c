/*
 * A098405  Expansion of (1-sqrt(1-8*x))/((1-x)*(4*x*sqrt(1-8*x))).
 * https://oeis.org/A098405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A098405: Expansion of (1-sqrt(1-8*x))/((1-x)*(4*x*sqrt(1-8*x))).
 * Recurrence: (n+1)*a(n) = (9*n+5)*a(n-1) - 4*(2*n+1)*a(n-2).
 * Initial values: a(0)=1, a(1)=7.  The recurrence yields all terms.  */

#include <stdlib.h>  /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Cache for computed values of a(n) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of cached elements        */

    /* Current binomial coefficient C(2k+1,k) and 2^k for last computed k */
    static seq_t coeff   = (seq_t)1;
    static seq_t pow2    = (seq_t)1;
    static idx_t  last_k = 0;      /* last k for which coeff and pow2 are valid */

    /* Initialise cache for a(0) only */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        cache_len = 1;
        coeff = (seq_t)1;             /* C(1,0) = 1   */
        pow2  = (seq_t)1;             /* 2^0 = 1      */
        last_k = 0;
    }

    /* Return cached value if already available */
    if (n < cache_len)
        return cache[n];

    /* Extend cache up to n */
    cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));

    for (idx_t i = cache_len; i <= n; ++i) {
        /* Update coefficient and power of two for k = i */
        if (i > 0) {
            idx_t k_prev = i - 1;              /* previous k value      */
            /* New coefficient: coeff * (2*k_prev+3)*(2*k_prev+2) / ((k_prev+1)*(k_prev+2)) */
            seq_t num = coeff * ((seq_t)(2 * k_prev + 3) * (seq_t)(2 * k_prev + 2));
            seq_t den = (seq_t)(k_prev + 1) * (seq_t)(k_prev + 2);
            coeff = num / den;
            pow2  = pow2 * 2;
        }
        /* Term to add: coeff * 2^i */
        seq_t term = coeff * pow2;
        cache[i] = cache[i - 1] + term;
    }

    cache_len = n + 1;
    last_k = n;
    return cache[n];
}
