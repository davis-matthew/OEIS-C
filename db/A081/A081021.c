/*
 * A081021  Even order Taylor coefficients at x = 0 of exp(
 *          (sqrt(2)-sqrt(-2*x^2+2))/(-2*x^2+2)^(1/2) ), odd order
 *          coefficients being equal to zero.
 * https://oeis.org/A081021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*(2n-3)*(2n-1)*a(n-1) -
 *            (2n-3)*(2n-1)*(12n^2-48n+47)*a(n-2) +
 *            4*(n-3)*(n-2)*(2n-5)^2*(2n-3)*(2n-1)*a(n-3) Initial values:
 *            a(1)=1, a(2)=12, a(3)=375
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[0] unused */
    static size_t   cap   = 0;    /* allocated capacity (bytes of seq_t) */
    static idx_t    last  = 0;    /* highest index computed */

    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1 */

    /* First call – initialise cache with a(1), a(2), a(3) */
    if (!cache)
    {
        cap = 4;                       /* indices 0 .. 3 */
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed */

        cache[1] = (seq_t)1;
        cache[2] = (seq_t)12;
        cache[3] = (seq_t)375;
        last    = 3;
    }

    /* Ensure cache large enough for index n */
    if ((size_t)n + 1 > cap)
    {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = n + 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
        {
            free(cache);
            cache = NULL;
            cap   = 0;
            last  = 0;
            return (seq_t)0;
        }
        if (new_cap > cap)
            memset(tmp + cap, 0, (new_cap - cap) * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* Already computed? */
    if (n <= last)
        return cache[n];

    /* Compute successive values up to n */
    for (idx_t k = last + 1; k <= n; ++k)
    {
        seq_t a_k_1 = cache[k-1];
        seq_t a_k_2 = cache[k-2];
        seq_t a_k_3 = cache[k-3];

        /* term1 = 3*(2k-3)*(2k-1)*a(k-1) */
        seq_t term1 = (seq_t)3;
        term1 *= (seq_t)(2 * (seq_t)k - 3);
        term1 *= (seq_t)(2 * (seq_t)k - 1);
        term1 *= a_k_1;

        /* coeff = 12*k^2 - 48*k + 47 */
        seq_t kk   = (seq_t)k;
        seq_t coeff = (seq_t)12 * kk * kk - (seq_t)48 * kk + (seq_t)47;

        /* term2 = (2k-3)*(2k-1)*coeff*a(k-2) */
        seq_t term2 = (seq_t)(2 * kk - 3);
        term2 *= (seq_t)(2 * kk - 1);
        term2 *= coeff;
        term2 *= a_k_2;

        /* term3 = 4*(k-3)*(k-2)*(2k-5)^2*(2k-3)*(2k-1)*a(k-3) */
        seq_t term3 = (seq_t)4;
        term3 *= (seq_t)(k - 3);
        term3 *= (seq_t)(k - 2);
        seq_t t2 = (seq_t)(2 * kk - 5);
        term3 *= t2;
        term3 *= t2;                     /* (2k-5)^2 */
        term3 *= (seq_t)(2 * kk - 3);
        term3 *= (seq_t)(2 * kk - 1);
        term3 *= a_k_3;

        /* a(k) = term1 - term2 + term3 */
        seq_t val = term1;
        val -= term2;
        val += term3;

        cache[k] = val;
    }

    last = n;
    return cache[n];
}
