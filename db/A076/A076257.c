/*
 * A076257  Coefficients of the polynomials in the numerator of 1/(1+x^2) and
 *          its successive derivatives, starting with the coefficient of the
 *          highest power of x.
 * https://oeis.org/A076257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 27, i32 n <= 52, i64 n <= 52, i128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A076257 : Coefficients of the numerators of successive derivatives
   of 1/(1+x^2).  The sequence is the concatenation of these coefficients
   (highest power first) for each derivative.
   For a given index n we find the corresponding derivative m and the
   offset t from the leading coefficient.  If t is odd the coefficient is 0;
   otherwise
       a(n) = (-1)^{m - t/2}  *  m!  *  binomial(m+1, m-t).
   The function implements this explicit formula. */

#include <stdint.h>

/* Helper to compute factorial( k ) as seq_t. */
static seq_t factorial(idx_t k)
{
    seq_t r = 1;
    for (idx_t i = 2; i <= k; ++i)
        r *= (seq_t)i;
    return r;
}

/* Helper to compute binomial( n, k ) as seq_t multiplicatively. */
static seq_t binomial_multiplicative(idx_t n, idx_t k)
{
    if (k > n - k)
        k = n - k;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r *= (seq_t)(n - k + i);
        r /= (seq_t)i;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Find smallest m with C(m) = (m+1)(m+2)/2 > n. */
    idx_t m = 0;
    while (1) {
        /* Use 128‑bit to avoid overflow of (m+1)*(m+2). */
        __uint128_t val = (__uint128_t)(m + 1) * (m + 2);
        val /= 2;
        if (val > n)
            break;
        ++m;
    }

    /* Offset within this derivative.  Preceding terms count is C(m-1)=m(m+1)/2. */
    idx_t prev = (m * (m + 1)) / 2;
    idx_t t = n - prev;               /* 0 ≤ t ≤ m */

    if (t & 1)                         /* odd offset → coefficient is 0 */
        return 0;

    idx_t k = m - t;                   /* power index of the coefficient   */
    idx_t exp = m - (t / 2);           /* exponent of (-1)                */
    seq_t sign = (exp & 1) ? -1 : 1;   /* (-1)^exp                       */

    seq_t fac = factorial(m);
    seq_t bin = binomial_multiplicative(m + 1, k);

    seq_t res = fac * bin;
    if (sign < 0)
        res = -res;
    return res;
}
