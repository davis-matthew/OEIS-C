/*
 * A014492  Inverse of 483rd cyclotomic polynomial.
 * https://oeis.org/A014492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of 1/Φ_483(x) where Φ_483(x) * is the
 *            483‑rd cyclotomic polynomial. The coefficients repeat * with
 *            period 483 and satisfy the recurrence * * a(0) = 1 * a(n) = −
 *            Σ_{k=1}^{min(n,264)} c_k · a(n−k) (n ≥ 1) * * where c_k are the
 *            coefficients of Φ_483(x). The array a[0…482] * i...
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static arrays – initialized once */
    static seq_t term[483];
    static int initialized = 0;

    if (!initialized)
    {
        initialized = 1;

        /* ----- 1. build Φ_483(x) via multiplication of (x^d − 1) factors ----- */
        /* divisors for positive and negative exponents (see discussion) */
        const idx_t pos_divs[4] = { 3, 7, 23, 483 };
        const idx_t neg_divs[4] = { 1, 21, 69, 161 };

        /* polynomial buffers – maximum degree 600 is ample for all ops */
        seq_t num[600] = {0};      /* numerator product (positive exponents) */
        idx_t deg_num = 0;
        num[0] = 1;               /* start with 1 */

        seq_t tmp[600];
        idx_t i, j;

        /* multiply by each (x^d − 1) with exponent +1 */
        for (i = 0; i < 4; ++i)
        {
            idx_t d = pos_divs[i];
            idx_t new_deg = deg_num + d;

            /* zero temporary buffer */
            for (j = 0; j <= new_deg; ++j) tmp[j] = 0;

            /* apply (x^d − 1): new[j] = old[j] (shifted) – old[j] */
            for (j = 0; j <= deg_num; ++j)
            {
                tmp[j]        -= num[j];
                tmp[j + d]    += num[j];
            }

            /* copy back */
            for (j = 0; j <= new_deg; ++j) num[j] = tmp[j];
            deg_num = new_deg;
        }

        seq_t den[600] = {0};      /* denominator product (negative exponents) */
        idx_t deg_den = 0;
        den[0] = 1;               /* start with 1 */

        /* multiply by each (x^d − 1) with exponent –1 */
        for (i = 0; i < 4; ++i)
        {
            idx_t d = neg_divs[i];
            idx_t new_deg = deg_den + d;

            for (j = 0; j <= new_deg; ++j) tmp[j] = 0;

            for (j = 0; j <= deg_den; ++j)
            {
                tmp[j]        -= den[j];
                tmp[j + d]    += den[j];
            }

            for (j = 0; j <= new_deg; ++j) den[j] = tmp[j];
            deg_den = new_deg;
        }

        /* ----- 2. divide N by D to obtain Φ_483(x) ----- */
        const idx_t deg_phi = deg_num - deg_den;   /* should be 264 */
        seq_t phi[600] = {0};

        seq_t rem[600];
        for (i = 0; i <= deg_num; ++i) rem[i] = num[i];

        /* long division: rem = rem / den */
        for (int64_t k = (int64_t)deg_num; k >= (int64_t)deg_den; --k)
        {
            seq_t coeff = rem[k];
            if (coeff != 0)
            {
                rem[k] = 0;
                phi[k - deg_den] = coeff;

                int64_t kk = k - deg_den;
                for (i = 0; i <= deg_den; ++i)
                    rem[kk + i] -= coeff * den[i];
            }
        }

        /* ----- 3. pre‑compute first 483 terms of the reciprocal series ----- */
        term[0] = 1;
        for (i = 1; i < 483; ++i)
        {
            seq_t sum = 0;
            idx_t maxk = (i < deg_phi) ? i : deg_phi;
            for (j = 1; j <= maxk; ++j)
                sum += phi[j] * term[i - j];
            term[i] = -sum;
        }
    }

    return term[n % 483];
}
