/*
 * A016340  156th cyclotomic polynomial.
 * https://oeis.org/A016340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   Φ_156(x) = ∏_{d|156} (x^d-1)^{μ(156/d)} (coefficients of
 *            A016340)
 * Range:     i8 n <= 48, i16 n <= 48, i32 n <= 48, i64 n <= 48, i128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t coeff[49];
static int initialized = 0;

/* Maximum possible degree of intermediate products:
   sum of positive divisors = 4+6+26+156 = 192,
   sum of negative divisors = 2+12+52+78 = 144 */
#define MAX_DEG 200   /* 200 > 192, 144 */

static void compute_coeffs(void)
{
    /* Polynomials for the numerator (μ=+1) and denominator (μ=-1) */
    seq_t num[MAX_DEG] = {0};
    seq_t den[MAX_DEG] = {0};
    size_t deg_num = 0, deg_den = 0;

    num[0] = 1;
    den[0] = 1;

    /* Divisors with μ=+1: 4, 6, 26, 156 */
    const idx_t pos_divs[4] = {4, 6, 26, 156};
    const idx_t neg_divs[4] = {2, 12, 52, 78};  /* μ = -1 */

    seq_t tmp[MAX_DEG] = {0};

    /* Multiply the numerator factors (x^d - 1) */
    for (size_t k = 0; k < 4; ++k) {
        idx_t d = pos_divs[k];
        for (size_t i = 0; i <= deg_num; ++i) {
            tmp[i]     -= num[i];         /* coefficient of -1 at degree i   */
            tmp[i + d] += num[i];         /* coefficient of +1 at degree i+d */
        }
        deg_num += (size_t)d;
        for (size_t i = 0; i <= deg_num; ++i) {
            num[i] = tmp[i];
            tmp[i] = 0;                     /* clear for next use */
        }
    }

    /* Multiply the denominator factors (x^d - 1) */
    for (size_t k = 0; k < 4; ++k) {
        idx_t d = neg_divs[k];
        for (size_t i = 0; i <= deg_den; ++i) {
            tmp[i]     -= den[i];
            tmp[i + d] += den[i];
        }
        deg_den += (size_t)d;
        for (size_t i = 0; i <= deg_den; ++i) {
            den[i] = tmp[i];
            tmp[i] = 0;
        }
    }

    /* Perform exact division: Φ_156 = num / den.
       The division is exact because the pair of products collapses
       to the cyclotomic polynomial. */
    seq_t rem[MAX_DEG] = {0};
    for (size_t i = 0; i <= deg_num; ++i) rem[i] = num[i];
    seq_t quot[49] = {0};
    size_t degR = deg_num;

    while (degR >= deg_den) {
        seq_t qcoef = rem[degR];
        if (qcoef == 0) {
            --degR;
            continue;
        }
        size_t qdeg = degR - deg_den;
        if (qdeg < 49) quot[qdeg] = qcoef;
        for (size_t i = 0; i <= deg_den; ++i) {
            rem[i + qdeg] -= qcoef * den[i];
        }
        while (degR > 0 && rem[degR] == 0) --degR;
    }

    /* Store final coefficients */
    for (size_t i = 0; i < 49; ++i) coeff[i] = quot[i];
}

seq_t a(idx_t n)
{
    if (!initialized) {
        compute_coeffs();
        initialized = 1;
    }
    return (n < 49) ? coeff[n] : 0;
}
