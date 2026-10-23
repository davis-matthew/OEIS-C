/*
 * A016381  319th cyclotomic polynomial.
 * https://oeis.org/A016381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 319th cyclotomic polynomial
 *            Φ_319(x)
 * Range:     i8 n <= 280, i16 n <= 280, i32 n <= 280, i64 n <= 280,
 *            i128 n <= 280, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 319 = 11 · 29.  For any n ≥ 0 the value is 0 for n > 280.  Computation is done once
 * per program run by factoring x^319‑1 and dividing by the product of all
 * proper cyclotomic factors.  The division exploits that the divisor is
 * monic, so all intermediate arithmetic stays integral and small.  */

#include <stdbool.h>
#include <string.h>

static inline void mul_poly(const seq_t *p, size_t lp, const seq_t *q, size_t lq,
                            seq_t *out)
{
    size_t i, j;
    for (i = 0; i < lp + lq; ++i) out[i] = 0;
    for (i = 0; i < lp; ++i)
        for (j = 0; j < lq; ++j)
            out[i + j] += p[i] * q[j];
}

seq_t a(idx_t n)
{
    /* pre‑computed coefficients of Φ_319(x) are stored in poly[0..280] */
    static bool ready = false;
    static seq_t poly[281];

    if (!ready) {
        /* Build the product A(x) = Φ_1(x)·Φ_11(x)·Φ_29(x) */
        seq_t p1[2] = { -1, 1 };          /* Φ_1(x) = x - 1 */

        seq_t p11[11];
        for (idx_t i = 0; i < 11; ++i) p11[i] = 1;   /* Φ_11(x) = 1 + … + x^10 */

        seq_t ptmp[12];
        mul_poly(p1, 2, p11, 11, ptmp); /* ptmp has degree 11 */

        seq_t p29[29];
        for (idx_t i = 0; i < 29; ++i) p29[i] = 1;   /* Φ_29(x) = 1 + … + x^28 */

        seq_t a[40];
        mul_poly(ptmp, 12, p29, 29, a); /* a has degree 39, leading coeff 1 */

        /* B(x) = x^319 - 1  (coeff[0] = -1, coeff[319] = 1) */
        seq_t B[320];
        for (idx_t i = 0; i < 320; ++i) B[i] = 0;
        B[319] = 1;
        B[0]   = -1;

        /* Polynomial long division:  B(x) ÷ a(x)  →  Φ_319(x)  */
        const idx_t degA = 39;
        idx_t i, j;
        for (i = 319; i >= degA; --i) {
            seq_t q = B[i];                 /* a[degA] = 1, so q = coefficient of result */
            if (q == 0) continue;
            poly[i - degA] = q;
            /* subtract q * a from B shifted by (i - degA) */
            for (j = 0; j <= degA; ++j)
                B[i - degA + j] -= a[j] * q;
        }
        /* Remaining lower coefficients of B must be zero; poly[0..280] now holds the result */
        ready = true;
    }

    if (n <= 280) return poly[n];
    return 0;
}
