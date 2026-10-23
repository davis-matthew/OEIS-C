/*
 * A014199  Inverse of 190th cyclotomic polynomial.
 * https://oeis.org/A014199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the power series expansion of
 *            1/Φ₁₉₀(x). * Φ₁₉₀(x) is the 190th cyclotomic polynomial, degree
 *            72. * The recurrence is * a(0) = 1 * a(n) = -∑_{i=1}^{72} φ[i] *
 *            a(n-i) (n ≥ 1) * where φ[i] (0 ≤ i ≤ 72) are the coefficients of
 *            Φ₁₉₀(x) (φ[0] = 1). * The sequence repe...
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Helper: multiply two polynomials.
 * A has degree degA, B has degree degB.
 * Result is stored in C and degC is updated. */
static void poly_mul(const seq_t *A, size_t degA,
                     const seq_t *B, size_t degB,
                     seq_t *C, size_t *degC)
{
    size_t i, j;
    /* clear C */
    for (i = 0; i <= degA + degB; ++i) C[i] = 0;
    for (i = 0; i <= degA; ++i) if (A[i] != 0) {
        for (j = 0; j <= degB; ++j) if (B[j] != 0) {
            C[i + j] += A[i] * B[j];
        }
    }
    /* compute degree */
    size_t d = degA + degB;
    while (d > 0 && C[d] == 0) d--;
    *degC = d;
}

/* Helper: exact polynomial division N / D.
 * N has degree degN, D has degree degD (leading coefficient 1).
 * Quotient Q gets computed (degree degQ = degN - degD). */
static void poly_div(const seq_t *N, size_t degN,
                     const seq_t *D, size_t degD,
                     seq_t *Q, size_t *degQ)
{
    seq_t tmp[217];               /* max degree 216 possible for N */
    size_t i, j, k;
    size_t qdeg = degN - degD;

    /* copy N to tmp */
    for (i = 0; i <= degN; ++i) tmp[i] = N[i];
    /* clear Q */
    for (k = 0; k <= qdeg; ++k) Q[k] = 0;

    for (i = degN; i >= degD; --i) {
        seq_t coeff = tmp[i];          /* current leading coefficient */
        Q[i - degD] = coeff;
        if (coeff == 0) continue;
        for (j = 0; j <= degD; ++j) {
            tmp[i - degD + j] -= coeff * D[j];
        }
    }
    *degQ = qdeg;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static data: coefficients of Φ190 and first period of the sequence */
    static bool init = false;
    static seq_t phi[73];   /* φ[0..72] */
    static seq_t seq[190];

    if (!init) {
        /* Build Φ190 via factorisation: product of (x^d-1) for selected divisors. */
        const int num_exp[4] = {2, 5, 19, 190};   /* divisors with exponent +1 */
        const int den_exp[4] = {1, 10, 38, 95};   /* divisors with exponent -1 */

        seq_t N[217] = {0}, D[217] = {0};
        size_t degN = 0, degD = 0;

        /* initialise polynomials to 1 */
        N[0] = 1;
        D[0] = 1;

        /* Multiply numerator factors */
        for (int idx = 0; idx < 4; ++idx) {
            int d = num_exp[idx];
            seq_t f[217] = {0};
            f[0] = -1;
            f[d] = 1;
            seq_t temp[217] = {0};
            size_t tempdeg = 0;
            poly_mul(N, degN, f, (size_t)d, temp, &tempdeg);
            /* copy back */
            for (size_t i = 0; i <= tempdeg; ++i) N[i] = temp[i];
            degN = tempdeg;
        }

        /* Multiply denominator factors */
        for (int idx = 0; idx < 4; ++idx) {
            int d = den_exp[idx];
            seq_t f[217] = {0};
            f[0] = -1;
            f[d] = 1;
            seq_t temp[217] = {0};
            size_t tempdeg = 0;
            poly_mul(D, degD, f, (size_t)d, temp, &tempdeg);
            for (size_t i = 0; i <= tempdeg; ++i) D[i] = temp[i];
            degD = tempdeg;
        }

        /* Divide to get Φ190(x) */
        seq_t Q[73] = {0};
        size_t degQ = 0;
        poly_div(N, degN, D, degD, Q, &degQ);   /* degQ should be 72 */

        for (size_t i = 0; i <= 72; ++i) phi[i] = Q[i];

        /* Compute the first 190 terms via recurrence */
        seq[0] = 1;
        for (size_t k = 1; k < 190; ++k) {
            seq_t sum = 0;
            size_t max_i = (k < 72) ? k : 72;
            for (size_t i = 1; i <= max_i; ++i) {
                if (phi[i] != 0) sum += phi[i] * seq[k - i];
            }
            seq[k] = -sum;
        }

        init = true;
    }

    /* Return periodic value */
    return seq[(size_t)(n % 190u)];
}
