/*
 * A014194  Inverse of 185th cyclotomic polynomial.
 * https://oeis.org/A014194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in 1/Φ_185(x). The cyclotomic
 *            polynomial Φ_185(x) has degree 144 and coefficients c[0..144];
 *            the sequence satisfies a(0)=1 and for n>0: a(n) = -∑_{k=1}^{144}
 *            c[k] * a(n-k). The sequence is periodic with period 185
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Multiply polynomial p by (x^k-1). p has degree deg. Result in out,
 * degree outdeg = deg + k. */
static void poly_mul_shift(const seq_t *p, size_t deg, size_t k,
                           seq_t *out, size_t *outdeg)
{
    size_t newdeg = deg + k;
    for (size_t i = 0; i <= newdeg; ++i) out[i] = 0;
    for (size_t i = 0; i <= deg; ++i) {
        out[i] -= p[i];
        out[i + k] += p[i];
    }
    *outdeg = newdeg;
}

/* Divide polynomial p by (x^k-1). p has degree deg. Result in out,
 * degree outdeg = deg - k.  The division is exact. */
static void poly_div_shift(const seq_t *p, size_t deg, size_t k,
                           seq_t *out, size_t *outdeg)
{
    seq_t temp[200];
    for (size_t i = 0; i <= deg; ++i) temp[i] = p[i];
    for (size_t i = 0; i <= deg - k; ++i) out[i] = 0;
    for (int i = (int)deg; i >= (int)k; --i) {
        seq_t coeff = temp[i];
        out[i - k] = coeff;
        temp[i] -= coeff;           /* subtract coeff * x^k  */
        temp[i - k] += coeff;       /* add coeff * 1       */
    }
    *outdeg = deg - k;
    /* After the loop, temp[0..k-1] should be zero. */
}

/* Compute coefficients of Φ_185(x).  Result is stored in f[0..144]. */
static void compute_phi185_coeffs(seq_t *f)
{
    seq_t buf1[200] = {0};
    size_t deg = 0;
    buf1[0] = 1;                     /* p(x) = 1 */

    seq_t buf2[200];
    size_t newdeg;

    /* multiply by (x^185 – 1) */
    poly_mul_shift(buf1, deg, 185, buf2, &newdeg);
    for (size_t i = 0; i <= newdeg; ++i) buf1[i] = buf2[i];
    deg = newdeg;

    /* divide by (x^37 – 1) */
    poly_div_shift(buf1, deg, 37, buf2, &newdeg);
    for (size_t i = 0; i <= newdeg; ++i) buf1[i] = buf2[i];
    deg = newdeg;

    /* divide by (x^5 – 1) */
    poly_div_shift(buf1, deg, 5, buf2, &newdeg);
    for (size_t i = 0; i <= newdeg; ++i) buf1[i] = buf2[i];
    deg = newdeg;

    /* multiply by (x – 1) */
    poly_mul_shift(buf1, deg, 1, buf2, &newdeg);
    for (size_t i = 0; i <= newdeg; ++i) buf1[i] = buf2[i];
    deg = newdeg;          /* should be 144 */

    for (size_t i = 0; i <= deg; ++i) f[i] = buf1[i];
}

/* Sequence computation */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t f[145];            /* coefficients of Φ_185(x) */
    static seq_t period[185];       /* period 0 … 184 */

    if (!initialized) {
        initialized = true;

        /* compute cyclotomic coefficients */
        compute_phi185_coeffs(f);

        /* generate one full period using the recurrence */
        period[0] = 1;
        for (size_t i = 1; i < 185; ++i) {
            seq_t sum = 0;
            for (size_t k = 1; k <= 144; ++k) {
                if (i >= k) sum -= f[k] * period[i - k];
            }
            period[i] = sum;
        }
    }

    return period[n % 185];
}
