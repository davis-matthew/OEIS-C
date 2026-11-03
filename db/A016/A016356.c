/*
 * A016356  209th cyclotomic polynomial.
 * https://oeis.org/A016356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 209th cyclotomic polynomial
 *            Φ_209(x)
 * Range:     i8 n <= 180, i16 n <= 180, i32 n <= 180, i64 n <= 180,
 *            i128 n <= 180, big any n
 * Validated: full, pass, 181 terms through n = 180 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute the coefficients of Φ_209(x) using the product formula
   Φ_209(x) = (x^209 - 1) / [(x - 1)(1 + x + … + x^10)(1 + x + … + x^18)] */
static void compute_209_coeffs(seq_t coeff[181]) {
    /* B(x) = 1 + x + ... + x^10,  degree 10, size 11 */
    seq_t B[11];
    for (idx_t i = 0; i <= 10; ++i) B[i] = (seq_t)1;

    /* C(x) = 1 + x + ... + x^18,  degree 18, size 19 */
    seq_t C[19];
    for (idx_t i = 0; i <= 18; ++i) C[i] = (seq_t)1;

    /* BC(x) = B(x) * C(x), degree 28, size 29 */
    seq_t BC[29];
    for (idx_t i = 0; i < 29; ++i) BC[i] = (seq_t)0;
    for (idx_t i = 0; i <= 10; ++i) {
        for (idx_t j = 0; j <= 18; ++j) {
            BC[i + j] += B[i] * C[j];
        }
    }

    /* D(x) = (x - 1) * BC(x), degree 29, size 30 */
    seq_t D[30];
    for (idx_t k = 0; k <= 29; ++k) {
        seq_t term1 = (k >= 1) ? BC[k - 1] : (seq_t)0;  /* x * BC */
        seq_t term2 = (k <= 28) ? BC[k] : (seq_t)0;     /* -BC */
        D[k] = term1 - term2;
    }

    /* Dividend A(x) = x^209 - 1, degree 209, size 210 */
    seq_t A[210];
    for (idx_t i = 0; i < 210; ++i) A[i] = (seq_t)0;
    A[209] = (seq_t)1;
    A[0] = (seq_t)-1;

    /* Temporary array for division */
    seq_t temp[210];
    for (idx_t i = 0; i <= 209; ++i) temp[i] = A[i];

    /* Quotient Q(x) = Φ_209(x), degree 180, size 181 */
    for (idx_t i = 209; i >= 29; --i) {
        seq_t q = temp[i];
        coeff[i - 29] = q;
        if (q != (seq_t)0) {
            /* Subtract q * D(x) shifted by (i - 29) */
            idx_t shift = i - 29;
            for (idx_t j = 0; j <= 29; ++j) {
                temp[shift + j] -= q * D[j];
            }
        }
    }

    /* The remainder temp[0..28] must be 0; ignore for safety */
}

/* forward declaration of helper to compute the coefficients only once */
static void compute_209_coeffs(seq_t coeff[181]);

seq_t a(idx_t n) {
    static bool initialized = false;
    static seq_t coeff[181];

    if (!initialized) {
        compute_209_coeffs(coeff);
        initialized = true;
    }

    if (n < 181) {
        return coeff[n];
    } else {
        /* Outside the polynomial degree all coefficients are zero */
        return (seq_t)0;
    }
}
