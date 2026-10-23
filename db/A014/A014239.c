/*
 * A014239  Inverse of 230th cyclotomic polynomial.
 * https://oeis.org/A014239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in 1/Φ₂₃₀(x). The power series is
 *            periodic with period 230
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */

struct Poly {
    int deg;                 /* largest exponent present */
    seq_t coeff[231];        /* coefficients, index = exponent; 0‑based */
};

/* Exact polynomial division: A = A / B  (B divides A exactly) */
static void
poly_div_exact(struct Poly *A, const struct Poly *B)
{
    seq_t R[231], Q[231];
    int i, j;

    for (i = 0; i <= A->deg; ++i) R[i] = A->coeff[i];
    for (i = 0; i <= A->deg; ++i) Q[i] = 0;

    int degB = B->deg;
    seq_t leadB = B->coeff[degB];

    for (i = A->deg; i >= degB; --i) {
        if (R[i] != 0) {
            seq_t coeff = R[i] / leadB;     /* exact because leadB == ±1 */
            Q[i - degB] = coeff;

            for (j = 0; j <= degB; ++j) {
                R[(i - degB) + j] -= coeff * B->coeff[j];
            }
        }
    }

    int newDeg = A->deg - degB;
    for (i = 0; i <= newDeg; ++i) A->coeff[i] = Q[i];
    A->deg = newDeg;
}

/* Static storage for the periodic values and initialization flag */
static seq_t period[230];
static int period_initialized = 0;

/* Compute the coefficients of 1/Φ₂₃₀(x) and store them in period[] */
static void
init_period(void)
{
    if (period_initialized) return;
    period_initialized = 1;

    struct Poly phis[231];
    int n;

    /* Φ₁(x) = x – 1 */
    phis[1].deg = 1;
    phis[1].coeff[0] = -1;
    phis[1].coeff[1] = 1;

    /* Compute Φₙ(x) for 2 ≤ n ≤ 230 */
    for (n = 2; n <= 230; ++n) {
        struct Poly P;
        P.deg = n;
        int k;
        for (k = 0; k <= n; ++k) P.coeff[k] = 0;
        P.coeff[0] = -1;
        P.coeff[n] = 1;

        for (int d = 1; d < n; ++d) {
            if (n % d == 0) {
                poly_div_exact(&P, &phis[d]);
            }
        }

        phis[n].deg = P.deg;
        for (k = 0; k <= P.deg; ++k) phis[n].coeff[k] = P.coeff[k];
    }

    /* Compute the inverse series of Φ₂₃₀(x) */
    const struct Poly *phi = &phis[230];
    seq_t a_coeff[230];
    a_coeff[0] = 1;
    int k;
    for (k = 1; k < 230; ++k) {
        seq_t sum = 0;
        int limit = (k <= phi->deg) ? k : phi->deg;
        int j;
        for (j = 1; j <= limit; ++j) {
            sum += phi->coeff[j] * a_coeff[k - j];
        }
        a_coeff[k] = -sum;
    }

    for (k = 0; k < 230; ++k) period[k] = a_coeff[k];
}

/* External function required by the build system */
seq_t a(idx_t n)
{
    init_period();
    idx_t r = n % 230;
    return period[r];
}
