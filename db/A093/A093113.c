/*
 * A093113  Coefficients of the solution to a functional equation.
 * https://oeis.org/A093113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The coefficients a(n) of the formal power series A(x) satisfy
 *            the functional equation A(4*x^2) = A(x) + A(-x) = -4 A(x) A(-x).
 *            From this it follows that * For even indices a(2m) = 2^(2m-1) *
 *            a(m) (equivalently a(n) = a(n/2) * 2^(n-1) when n is even). *
 *            For odd indices n = 2k+1 (k ≥ 1) one obtains...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"

/* Memoisation cache */
static seq_t *cache = NULL;
static idx_t  cache_cap   = 0;   /* allocated size of cache */
static idx_t  cache_used  = 0;   /* highest index currently computed (≥0) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety, never called for n=0 */

    /* Ensure capacity for indices up to n */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        if (new_cap < 16) new_cap = 16;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure – unlikely */
        /* initialise new elements to zero */
        for (idx_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values from cache_used+1 up to n */
    for (idx_t i = cache_used + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = 1;
        } else if ((i & 1) == 0) {            /* even i */
            idx_t half = i >> 1;              /* i/2 */
            cache[i] = cache[half] * pow2(i - 1);
        } else {                              /* odd i > 1 */
            idx_t  m   = i + 1;              /* even index */
            seq_t  a_m = cache[m >> 1] * pow2(i);  /* a(m) = a[(m)/2] * 2^(m-1) = a[(i+1)/2] * 2^i */

            /* Sum over all i=2..(m-2) (i.e. 2..n-1) skipping j=1 and j=n */
            seq_t pos = 0;   /* sum of terms with positive sign */
            seq_t neg = 0;   /* sum of terms with negative sign */
            for (idx_t j = 2; j <= i - 1; ++j) {
                idx_t j2 = m - j;                     /* j2 = m - j */
                seq_t prod = cache[j] * cache[j2];
                /* sign = (-1)^(m-j) ; m is even */
                if (((m - j) & 1) == 0) {
                    pos += prod;                     /* positive sign */
                } else {
                    neg += prod;                     /* negative sign */
                }
            }
            /* a(i) = ( a_m + 2*pos - 2*neg ) / 4 */
            seq_t numerator = a_m + ((seq_t)2) * pos;
            numerator -= ((seq_t)2) * neg;           /* works because the result is non‑negative */
            cache[i] = numerator / ((seq_t)4);
        }
    }
    cache_used = n;
    return cache[n];
}
