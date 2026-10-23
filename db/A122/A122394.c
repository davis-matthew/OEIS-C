/*
 * A122394  Dimension of 5-variable non-commutative harmonics (Hausdorff
 *          derivative). The dimension of the space of non-commutative
 *          polynomials in 5 variables which are killed by all symmetric
 *          differential operators (where for a monomial w, d_{xi} ( w ) = sum
 *          over all subwords of w deleting xi once).
 * https://oeis.org/A122394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(n)=5*a(n-1)+p(n) where *
 *            p[0..15]={1,-1,-1,0,0,1,1,1,-1,-1,-1,0,0,1,1,-1} * and p(n)=0
 *            for n>15. This is equivalent to the generating * function G(q) =
 *            (1-q)(1-q^2)(1-q^3)(1-q^4)(1-q^5)/(1-5q)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Coefficients of the numerator polynomial */
static const int p[16] = {
    1,-1,-1,0,0,1,1,1,-1,-1,-1,0,0,1,1,-1
};

static seq_t *cache = NULL;      /* memoized values of a(n) */
static idx_t  cache_size = 0;    /* number of values stored */
static idx_t  cache_cap   = 0;    /* allocated capacity */

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Initialise cache with a(0)=1 */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failed */
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* Grow the cache if needed */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (n >= new_cap) new_cap <<= 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0;   /* allocation failed */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val = cache[i-1] * (seq_t)5;
        if (i <= 15) {
            int coeff = p[i];
            if (coeff >= 0)
                val += (seq_t)coeff;
            else
                val -= (seq_t)(-coeff);
        }
        cache[i] = val;
    }
    cache_size = n + 1;

    return cache[n];
}
