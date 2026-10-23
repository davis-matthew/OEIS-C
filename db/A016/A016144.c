/*
 * A016144  Inverse of 2135th cyclotomic polynomial.
 * https://oeis.org/A016144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the formal power series expansion
 *            of 1 / Φ_{2135}(x). The sequence is periodic with period 2135
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Period of the sequence and its cyclotomic degree.  These must be compile‑time
   constants; using #define ensures this in both C and C++. */
#define PERIOD 2135U
#define PHI    1440U

/* --------------------------------------------------------------------- */
/* Polynomial arithmetic (integer coefficients).  The arrays are assumed
   to be zeroed before the call. */
static void poly_mul(const int *A, size_t da,
                     const int *B, size_t db,
                     int *C)
{
    for (size_t i = 0; i <= da; ++i) {
        if (A[i] == 0) continue;
        for (size_t j = 0; j <= db; ++j) {
            if (B[j] == 0) continue;
            C[i + j] += A[i] * B[j];
        }
    }
}

/* Exact division of NUM by DEN when DEN[dd] == 1.  Quotient Q must be zeroed
   before the call.  Result has degree dn - dd. */
static void poly_div_exact(const int *NUM, size_t dn,
                           const int *DEN, size_t dd,
                           int *Q)
{
    int temp[2209];        /* maximum degree of NUM (≥ 2208) */
    for (size_t i = 0; i <= dn; ++i)
        temp[i] = NUM[i];

    for (int i = (int)dn; i >= (int)dd; --i) {
        int coeff = temp[i];
        if (coeff != 0) {
            Q[i - (int)dd] = coeff;
            for (size_t j = 0; j <= dd; ++j)
                temp[(i - (int)dd) + j] -= coeff * DEN[j];
        }
    }
}

/* --------------------------------------------------------------------- */
/* Compute the coefficients of Φ_{2135}(x).  The coefficients are stored
   in the array coeffs[0 .. PHI].  coeffs[0] == 1. */
static void compute_phi_coeffs(int coeffs[PHI + 1])
{
    /* 2135 = 5 * 7 * 61   (squarefree).  Use  
       Φ_n(x) = ∏_{d|n} (x^d - 1)^{μ(n/d)}. */

    /* Numerator factors (exponent +1). */
    int f1[2136] = {0};          /* x^2135 - 1 */
    f1[0] = -1; f1[2135] = 1;
    int f5[6]   = {0};          /* x^5 - 1 */
    f5[0] = -1; f5[5] = 1;
    int f7[8]   = {0};          /* x^7 - 1 */
    f7[0] = -1; f7[7] = 1;
    int f61[62] = {0};          /* x^61 - 1 */
    f61[0] = -1; f61[61] = 1;

    /* Denominator factors (exponent -1). */
    int g1[2]    = {0};          /* x - 1 */
    g1[0] = -1; g1[1] = 1;
    int g35[36]  = {0};          /* x^35 - 1 */
    g35[0] = -1; g35[35] = 1;
    int g305[306]= {0};          /* x^305 - 1 */
    g305[0] = -1; g305[305] = 1;
    int g427[428]= {0};          /* x^427 - 1 */
    g427[0] = -1; g427[427] = 1;

    /* Build the numerator polynomial: (x^2135-1)(x^5-1)(x^7-1)(x^61-1) */
    int num1[2209] = {0};
    poly_mul(f1, 2135, f5, 5, num1);          /* degree 2140 */
    int num2[2209] = {0};
    poly_mul(num1, 2140, f7, 7, num2);        /* degree 2147 */
    int num3[2209] = {0};
    poly_mul(num2, 2147, f61, 61, num3);      /* degree 2208 */

    /* Build the denominator polynomial: (x-1)(x^35-1)(x^305-1)(x^427-1) */
    int den1[769] = {0};
    poly_mul(g1, 1, g35, 35, den1);           /* degree 36 */
    int den2[769] = {0};
    poly_mul(den1, 36, g305, 305, den2);     /* degree 341 */
    int den3[769] = {0};
    poly_mul(den2, 341, g427, 427, den3);    /* degree 768 */

    /* Divide numerator by denominator to obtain Φ_{2135}. */
    int phi[PHI + 1] = {0};
    poly_div_exact(num3, 2208, den3, 768, phi);

    /* Copy the result into the output array. */
    for (size_t i = 0; i <= PHI; ++i)
        coeffs[i] = phi[i];
}

/* --------------------------------------------------------------------- */
/* Compute the periodic sequence once.  The array is indexed by
   unsigned indices [0 .. PERIOD-1]. */
static const seq_t *precompute_period(void)
{
    static seq_t period[PERIOD];
    static int coeff[PHI + 1];
    static int ready = 0;

    if (ready) return period;

    compute_phi_coeffs(coeff);

    period[0] = (seq_t)1;

    for (idx_t n = 1; n < PERIOD; ++n) {
        seq_t sum = 0;
        size_t limit = (n < PHI) ? (size_t)n : (size_t)PHI;
        for (size_t k = 1; k <= limit; ++k)
            sum += (seq_t)coeff[k] * period[(size_t)(n - k)];
        period[(size_t)n] = -sum;
    }

    ready = 1;
    return period;
}

/* --------------------------------------------------------------------- */
/* Public function required by the framework. */
seq_t a(idx_t n)
{
    const seq_t *period = precompute_period();
    return period[(size_t)(n % PERIOD)];
}
