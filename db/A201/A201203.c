/*
 * A201203  Alternating row sums of triangle A201201: first associated monic
 *          Laguerre-Sonin(e) polynomials with parameter alpha=1 evaluated at
 *          x=-1.
 * https://oeis.org/A201203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 9, i64 n <= 17, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -(2*n+3)*a(n-1) - n*(n+1)*a(n-2)
   with a(0) = 1, a(-1) = 0 (used for n=1) */
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* Ensure base case a(0)=1 is stored */
    if (cache_len == 0) {
        cache = (seq_t *)calloc(1, sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure fallback */
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    if (n < cache_len)
        return cache[n];

    /* Grow cache to hold indices up to n */
    idx_t old_len = cache_len;
    cache_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
    if (!tmp) return (seq_t)0;                  /* allocation failure fallback */
    cache = tmp;

    /* Compute new terms from old_len up to n inclusive */
    for (idx_t k = old_len; k <= n; ++k) {
        if (k == 1) {
            /* a(1) = -(2*1+3)*a(0)  [a(-1)=0] */
            cache[1] = -( (seq_t)(2 * (seq_t)1 + 3) * cache[0] );
        } else {   /* k >= 2 */
            seq_t coeff  = (seq_t)(2 * (seq_t)k + 3);          /* 2*k+3 */
            seq_t coeff2 = (seq_t)k * ((seq_t)k + 1);          /* k*(k+1) */
            cache[k] = - (coeff * cache[k-1] + coeff2 * cache[k-2]);
        }
    }
    return cache[n];
}
