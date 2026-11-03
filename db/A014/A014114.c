/*
 * A014114  Inverse of 105th cyclotomic polynomial.
 * https://oeis.org/A014114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 97 terms through n = 996 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the coefficient of x^n in the power‑series expansion of the reciprocal
   of the 105th cyclotomic polynomial Φ105(x).  It satisfies
   a(0) = 1 and for n ≥ 1
        a(n) = –∑_{k=1}^{48} φ_k · a(n–k),
   where φ_k are the coefficients of Φ105(x) (φ_0 = 1).  The values repeat
   with period 105.  This implementation builds φ_k once and then
   evaluates a(n) by this periodic recurrence. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>   /* for memcpy, memset */
#include "../utils/mem.h"

static void mul_by_xd_minus1(seq_t *poly, idx_t *deg, idx_t d)
{
    /* Multiply the polynomial by (x^d – 1).
       poly[0..*deg] contains the current polynomial.
       After the call poly[0..(*deg)+d] holds the result and *deg is updated. */
    static seq_t tmp[200];
    idx_t newdeg = *deg + d;
    /* clear tmp to zero for indices up to newdeg */
    for (idx_t i = 0; i <= newdeg; ++i) tmp[i] = 0;
    for (idx_t i = 0; i <= *deg; ++i) {
        seq_t val = poly[i];
        tmp[i]     -= val;     /* multiply by –1   */
        tmp[i + d] += val;     /* multiply by x^d   */
    }
    /* copy back to poly */
    for (idx_t i = 0; i <= newdeg; ++i) poly[i] = tmp[i];
    *deg = newdeg;
}

/* Divide the monic polynomial 'den' (degree deg_den) from 'num' (degree deg_num).
   The result is stored in 'quot' and its degree in *deg_quot.  The division
   is exact because den is monic and divides num. */
static void poly_divide_monic(const seq_t *num, idx_t deg_num,
                              const seq_t *den, idx_t deg_den,
                              seq_t *quot, idx_t *deg_quot)
{
    static seq_t tmp[200];
    mem_copy(tmp, num, (deg_num + 1) * sizeof(seq_t));
    /* initialize quotient to zero */
    for (idx_t i = 0; i <= deg_num - deg_den; ++i) quot[i] = 0;
    for (idx_t i = deg_num; i >= deg_den; --i) {
        seq_t coeff = tmp[i];
        if (coeff == 0) continue;
        quot[i - deg_den] = coeff;
        /* subtract coeff * den shifted by (i–deg_den) */
        for (idx_t j = 0; j <= deg_den; ++j) {
            tmp[i - deg_den + j] -= coeff * den[j];
        }
    }
    *deg_quot = deg_num - deg_den;
}

/* Compute the coefficients of Φ105(x).  The coefficients are stored in
   phi_coeff[0..48].  phi_coeff[0] will be set to 1. */
static void compute_phi105(seq_t phi_coeff[49])
{
    static seq_t num[200], den[200];
    idx_t deg_num = 0, deg_den = 0;
    /* initialise to the constant polynomial 1 */
    num[0] = den[0] = 1;
    /* numerator factors: (x^3–1)(x^5–1)(x^7–1)(x^105–1) */
    const idx_t num_factors[] = {3, 5, 7, 105};
    for (size_t i = 0; i < sizeof(num_factors) / sizeof(num_factors[0]); ++i)
        mul_by_xd_minus1(num, &deg_num, num_factors[i]);
    /* denominator factors: (x–1)(x^15–1)(x^21–1)(x^35–1) */
    const idx_t den_factors[] = {1, 15, 21, 35};
    for (size_t i = 0; i < sizeof(den_factors) / sizeof(den_factors[0]); ++i)
        mul_by_xd_minus1(den, &deg_den, den_factors[i]);

    /* divide numerator by denominator to obtain Φ105 */
    static seq_t quot[200];
    idx_t deg_quot;
    poly_divide_monic(num, deg_num, den, deg_den, quot, &deg_quot);

    /* store the coefficients (deg_quot should be 48) */
    for (idx_t i = 0; i <= deg_quot; ++i)
        phi_coeff[i] = quot[i];
}

seq_t a(idx_t n)
{
    /* Cache one full period (length 105).  The period and the polynomial
       coefficients are computed lazily on first call. */
    static seq_t period[105];
    static bool ready = false;

    if (!ready) {
        /* 1. compute coefficients φ_k of Φ105(x) */
        static seq_t phi[49];
        compute_phi105(phi);

        /* 2. compute the period by the recurrence a(n) = –∑ φ_k * a(n-k) */
        period[0] = 1;  /* a(0) = 1 */
        for (idx_t i = 1; i < 105; ++i) {
            seq_t sum = 0;
            idx_t kmax = (i < 48) ? i : 48;
            for (idx_t k = 1; k <= kmax; ++k)
                sum -= phi[k] * period[i - k];
            period[i] = sum;
        }
        ready = true;
    }

    return period[n % 105];
}
