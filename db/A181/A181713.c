/*
 * A181713  Solutions of A181712(n)=0.
 * https://oeis.org/A181713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the n-th positive integer k such that
 *          {k*phi} < {3*k*phi},  where phi = (1+sqrt(5))/2
 *          (i.e. k for which the inequality in the Beatty sequence
 *          A181712 holds).
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;      /* array of terms */
    static size_t  cap   = 0;        /* current capacity of array    */
    static idx_t   have  = 0;        /* number of terms stored */
    static idx_t   next_k = 1;       /* next k value to test */

    if (n == 0) return (seq_t)0;    /* off‑by‑one safety (offset = 1) */

    /* If we already know the n-th term, return it */
    if (have >= n) return cache[n-1];

    /* Extend the cache if necessary */
    if (cap < (size_t)n) {
        size_t new_cap = cap ? cap*2 : 128;
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure (unlikely) */
        cache = tmp;
        cap   = new_cap;
    }

    /* Constants used in the test */
    static const long double phi      = (1.0L + sqrtl(5.0L)) / 2.0L;
    const long double one_third = 1.0L / 3.0L;
    const long double half      = 0.5L;
    const long double two_third = 2.0L / 3.0L;
    const long double eps       = 1e-12L;          /* safety margin */

    /* Generate terms until we have at least n values */
    while (have < n) {
        long double prod  = (long double)next_k * phi;
        long double alpha = prod - floorl(prod);     /* fractional part */

        /* {k*phi} < {3*k*phi}  <=>  alpha in [0,1/3) ∪ (1/2,2/3)   */
        if ((alpha < one_third - eps) ||
            (alpha > half + eps && alpha < two_third - eps)) {
            cache[have++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[n-1];
}
