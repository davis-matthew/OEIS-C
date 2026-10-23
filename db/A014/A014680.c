/*
 * A014680  Inverse of 671st cyclotomic polynomial.
 * https://oeis.org/A014680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the power series expansion of
 *            1/Φ_{671}(x)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Polynomial multiplication:  res = a * b
   degA and degB are the degrees of the input polynomials. */
static void poly_mul(const seq_t *a, size_t degA,
                     const seq_t *b, size_t degB,
                     seq_t *res)
{
    size_t degR = degA + degB;
    for (size_t i = 0; i <= degR; ++i) res[i] = 0;
    for (size_t i = 0; i <= degA; ++i) {
        if (a[i] == 0) continue;
        for (size_t j = 0; j <= degB; ++j) {
            res[i + j] += a[i] * b[j];
        }
    }
}

/* Exact polynomial division:  numer = denom * quot
   degN and degD are the degrees of numerator and denominator
   (numer is divisible by denom, leading coefficient of denom is 1). */
static void poly_div(const seq_t *numer,
                     size_t degN,
                     const seq_t *denom,
                     size_t degD,
                     seq_t *quot)
{
    /* remainder array, max degree 671 (x^671 - 1) */
    seq_t rem[672];
    for (size_t i = 0; i <= degN; ++i) rem[i] = numer[i];

    size_t degQ = degN - degD;
    for (size_t i = 0; i <= degQ; ++i) quot[i] = 0;

    for (int i = (int)degN; i >= (int)degD; --i) {
        seq_t coef = rem[i];
        size_t j = (size_t)(i - degD);
        quot[j] = coef;
        for (size_t k = 0; k <= degD; ++k) {
            rem[j + k] -= coef * denom[k];
        }
    }
    /* remainder should be zero; ignored for brevity */
}

/* Memoized result array of one period */
static seq_t period[671];

/* Compute the first 671 coefficients of the series expansion of 1/Φ_{671}(x) */
static void compute_period(void)
{
    /* Φ_1(x) = x - 1 */
    seq_t phi1[2];
    phi1[0] = -1;
    phi1[1] = 1;

    /* Φ_11(x) = 1 + x + ... + x^10 */
    seq_t phi11[11];
    for (size_t i = 0; i < 11; ++i) phi11[i] = 1;

    /* Φ_61(x) = 1 + x + ... + x^60 */
    seq_t phi61[61];
    for (size_t i = 0; i < 61; ++i) phi61[i] = 1;

    /* Compute denominator = Φ_1 * Φ_11 * Φ_61 */
    seq_t prod1[12];          /* degree 1 + 10 = 11 -> size 12 */
    poly_mul(phi1, 1, phi11, 10, prod1);

    seq_t denom[72];          /* degree 11 + 60 = 71 -> size 72 */
    poly_mul(prod1, 11, phi61, 60, denom);

    /* Numerator = x^671 - 1 */
    seq_t numer[672];
    for (size_t i = 0; i < 672; ++i) numer[i] = 0;
    numer[0] = -1;
    numer[671] = 1;

    /* Φ_671 = (x^671 - 1) / denom */
    seq_t phi671[601];        /* degree 600 -> size 601 */
    poly_div(numer, 671, denom, 71, phi671);

    /* Build the sequence via recurrence a(k) + Σ A[i] * a(k-i) = 0,  k>0
       where A[i] = coeff of Φ_671(x) (i≥1).  Deg = 600. */
    period[0] = 1;            /* a(0) = 1 */
    for (size_t k = 1; k < 671; ++k) {
        seq_t sum = 0;
        size_t max_i = (k < 600 ? k : 600);
        for (size_t i = 1; i <= max_i; ++i) {
            sum += phi671[i] * period[k - i];
        }
        period[k] = -sum;
    }
}

/* Public function providing the OEIS sequence */
seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
        compute_period();
        initialized = 1;
    }
    return period[n % 671];
}
