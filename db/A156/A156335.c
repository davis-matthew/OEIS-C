/*
 * A156335  G.f.: A(x) = exp( Sum_{n>=1} 2^[(n^2+1)/2]*x^n/n ), a power series
 *          in x with integer coefficients.
 * https://oeis.org/A156335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A156335: a(0)=1, a(n)=1/n * sum_{k=1..n} 2^{floor((k^2+1)/2)}*a(n-k)  */

/* Dependencies: Standard integer types, stdlib for memory allocation. */

#include <stdint.h>
#include <stdlib.h>

/* Static cache for memoised values */
static seq_t *cache = NULL;     /* array of computed a(n) values */
static idx_t  cache_size = 0;   /* current allocation size */
static idx_t  last_computed = 0;/* highest n that has been computed */

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    /* Initialise cache if first call */
    if (cache == NULL) {
        cache_size = (n > 0) ? n + 1 : 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* failure fallback */
        cache[0] = (seq_t)1;                  /* a(0) */
        last_computed = 0;
    }

    /* Extend cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache,
                                      new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;     /* failure fallback */
        cache = tmp;
        cache_size = new_size;
    }

    /* If requested n already computed, return it immediately */
    if (n <= last_computed)
        return cache[n];

    /* Compute all intermediate values up to n inclusively */
    for (idx_t m = last_computed + 1; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= m; ++k) {
            idx_t exp = (k * k + 1) / 2;   /* floor((k^2+1)/2) */
            /* Multiply a(m-k) by 2^exp via a left shift */
            seq_t term = cache[m - k] << exp;
            sum += term;
        }
        /* a(m) = sum / m */
        cache[m] = sum / m;
    }

    last_computed = n;
    return cache[n];
}
