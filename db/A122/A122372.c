/*
 * A122372  Dimension of 8-variable non-commutative harmonics (twisted
 *          derivative). The dimension of the space of non-commutative
 *          polynomials in 8 variables which are killed by all symmetric
 *          differential operators (where for a monomial w, d_{xi} ( xi w ) =
 *          w and d_{xi} ( xj w ) = 0 for i/=j).
 * https://oeis.org/A122372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence * a(n) = 28*a(n-1) -
 *            316*a(n-2) + 1845*a(n-3) - 5925*a(n-4) * + 10190*a(n-5) -
 *            8249*a(n-6) + 2119*a(n-7) * with the initial values
 *            a(0)=1,a(1)=7,a(2)=55,a(3)=438, *
 *            a(4)=3498,a(5)=27962,a(6)=223604. These are obtained from * the
 *            generating function condition *...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache to store already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity            */
    static size_t cache_len = 0;   /* number of terms computed      */

    /* coefficients from the numerator and denominator of the GF */
    static const int N_coeff[7] = {  1, -21,  175, -735, 1624, -1764,  720};
    static const int D_coef[7] = { 28, -316, 1845, -5925, 10190, -8249, 2119};

    /* grow cache if necessary */
    size_t needed = (size_t)n + 1;         /* number of terms we need        */
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = cache_len; i < needed; ++i) {
        seq_t sum = 0;

        /* add the N‑coeff term (only for i ≤ 6) */
        if (i <= 6) {
            int nc = N_coeff[(int)i];
            if (nc >= 0)
                sum += (seq_t)nc;
            else
                sum -= (seq_t)(-nc);   /* subtract absolute value   */
        }

        /* add the D‑coeff terms */
        for (int k = 1; k <= 7; ++k) {
            if (i < (size_t)k) break;
            int dc = D_coef[k - 1];
            seq_t term = cache[i - k] * (seq_t)((dc >= 0) ? dc : -dc);
            if (dc >= 0)
                sum += term;
            else
                sum -= term;
        }

        cache[i] = sum;
    }

    cache_len = needed;
    return cache[(size_t)n];
}
