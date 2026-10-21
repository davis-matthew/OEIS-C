/*
 * A064325  Generalized Catalan numbers C(-3; n).
 * https://oeis.org/A064325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   Recurrence a(0) = 1, a(1) = 1 for n >= 2: 4*n*a(n) +
 *            (47*n-72)*a(n-1) + 6*(-2*n+3)*a(n-2) = 0 Thus: a(n) = (
 *            -((47*n-72)*a(n-1) + 6*(-2*n+3)*a(n-2)) ) / (4*n) The
 *            implemented code uses a memoised table and a larger intermediate
 *            type ( __int128 for C builds, seq_t itself for C++ builds) to
 *            avoid o...
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 11, i64 n <= 20, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
    /* In C++ mode seq_t may be an arbitrary‑precision class; use it directly. */
    typedef seq_t big_t;
#else
    /* In pure C we use a built‑in wider type if available. */
    #if defined(__SIZEOF_INT128__)
        typedef __int128 big_t;
    #else
        typedef long long big_t;
    #endif
#endif

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t max_known = 0;

    /* Special case for n == 0 or 1 */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        max_known = 1;
    }

    if (n <= max_known)
        return cache[n];

    /* Grow cache if needed */
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing values using recurrence */
    for (idx_t k = max_known + 1; k <= n; ++k) {
        big_t nn = (big_t)k;

        /* coeff1 = 47*n - 72 */
        big_t coeff1 = (big_t)47 * nn - (big_t)72;

        /* coeff2 = 6 * (-2*n + 3) = -12*n + 18 */
        big_t coeff2 = (big_t)6 * (((big_t)(-2) * nn) + (big_t)3);

        /* term1 = coeff1 * a(k-1) */
        big_t term1 = coeff1 * cache[k - 1];

        /* term2 = coeff2 * a(k-2) */
        big_t term2 = coeff2 * cache[k - 2];

        /* numerator = -(term1 + term2) */
        big_t num = -(term1 + term2);

        /* denominator = 4*n */
        big_t denom = (big_t)4 * nn;

        /* a(k) = num / denom (exact division) */
        cache[k] = (seq_t)(num / denom);
    }

    max_known = n;
    return cache[n];
}
