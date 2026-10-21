/*
 * A060629  1/2+Sum_{n >= 1} a(n)*x^(2*n)/(4^n*(2*n)!) = 1/Pi*EllipticK(x).
 * https://oeis.org/A060629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n,n) * binomial(2*n-1,n) * (2*n)! / 4^n The
 *            recurrence derived from the formula is a(1) = 1 a(n+1) = a(n) *
 *            2 * (2*n+1)^3 / (n+1) This recurrence holds exactly for all n ≥
 *            1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n) for the OEIS sequence A060629.
   Uses memoisation to avoid recomputation when called multiple times. */
seq_t a(idx_t n) {
    /* 0 is unused; use a(1) = 1 as the starting value. */
    if (n == 0) return 0;

    static seq_t *cache = NULL;          /* dynamic array of computed values */
    static idx_t       cache_max = 0;    /* highest index computed so far */
    static size_t      cache_cap = 0;    /* allocated capacity of 'cache' */

    /* initialise allocation on first call or when small n is requested */
    if (cache == NULL) {
        cache_cap = n + 2;               /* at least up to n */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[1] = (seq_t)1;            /* a(1) */
        cache_max = 1;
    }

    /* grow the array if n exceeds current capacity */
    if ((size_t)(n + 1) > cache_cap) {
        cache_cap = n + 2;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* compute any missing terms up to n */
    for (idx_t i = cache_max; i < n; ++i) {
        /* factor = 2 * (2*i+1)^3 */
        seq_t k = (seq_t)(2 * i + 1);
        seq_t factor = k * k * k;
        factor *= (seq_t)2;

        seq_t val = cache[i] * factor;
        val /= (seq_t)(i + 1);          /* guaranteed integer division */

        cache[i + 1] = val;
    }
    cache_max = n;
    return cache[n];
}
