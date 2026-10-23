/*
 * A192358  Coefficients of x in the reduction of the polynomial
 *          p(n,x)=(1/2)((x+3)^n+(x-3)^n) by x^2->x+1.
 * https://oeis.org/A192358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*a(n-1) + 19*a(n-2) - 20*a(n-3) - 55*a(n-4)
 * Initial values: a(1) = 0, a(2) = 1, a(3) = 1, a(4) = 29
 *
 * This implementation caches previously computed terms and uses the
 * recurrence to compute new ones in O(1) per term.  All arithmetic is
 * performed with the provided seq_t type, which may be an arbitrary‑precision
 * integer.  The recurrence is rearranged to avoid unsigned underflow
 * (term1 >= term2 for all n ≥ 5).
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* For safety, treat n == 0 as 0; the sequence is defined for n ≥ 1. */
    if (n == 0ULL) return (seq_t)0;

    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;   /* number of allocated elements (1‑based) */
    static idx_t  last_computed = 0;/* highest index computed so far */

    /* Initialise cache with first four terms if needed */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t *)malloc((size_t)(cache_size + 1) * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* out of memory - unlikely */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)29;
        last_computed = 4;
    }

    /* Grow cache if n exceeds current capacity */
    if (n > cache_size) {
        idx_t new_size = n;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(new_size + 1) * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* out of memory */
        cache = tmp;
        cache_size = new_size;
    }

    /* Return cached value if already computed */
    if (n <= last_computed) {
        return cache[n];
    }

    /* Compute any missing terms up to n using the recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t term1 = cache[i-1] * (seq_t)2 + cache[i-2] * (seq_t)19;
        seq_t term2 = cache[i-3] * (seq_t)20 + cache[i-4] * (seq_t)55;
        cache[i] = term1 - term2; /* safe: term1 ≥ term2 for n ≥ 5 */
    }
    last_computed = n;

    return cache[n];
}
