/*
 * A015362  Inverse of 1353rd cyclotomic polynomial.
 * https://oeis.org/A015362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) = coefficient of x^n in 1/Φ₁₃₅₃(x), periodic with period
 *            1353. * This implementation constructs the cyclotomic polynomial
 *            Φ₁₃₅₃(x) * by exact integer division and then computes the
 *            coefficients of * its reciprocal series up to the period
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The calculation is performed only once, on the first call to a(n). */
#include <stddef.h>   /* size_t */

#define PERIOD   1353U
#define DEG_A    1408   /* sum of exponents with μ(1353/d)=+1 : 3+11+41+1353 */
#define DEG_B    608    /* sum of exponents with μ(1353/d)=-1 : 1+33+123+451  */
#define DEG_Q    800    /* degree of Φ₁₃₅₃(x) */

/*  Temporary arrays for the division and for constructing the polynomials. */
static seq_t polyA[DEG_A + 1];
static seq_t polyB[DEG_B + 1];
static seq_t phi[DEG_Q + 1];        /* coefficients of Φ₁₃₅₃(x) */
static seq_t seq_series[PERIOD];    /* coefficients of 1/Φ₁₃₅₃(x) */
static int initialized = 0;

/*  Multiply poly[0..deg] by (x^d - 1) and store the result back in poly. */
static void multiply_by_factor(seq_t *poly, size_t *deg, size_t d)
{
    size_t new_deg = *deg + d;
    seq_t tmp[DEG_A + 1] = {0};

    for (size_t i = 0; i <= *deg; ++i) {
        tmp[i] += poly[i];           /* term from the original polynomial */
        tmp[i + d] -= poly[i];       /* term from the -1 part */
    }
    for (size_t i = 0; i <= new_deg; ++i)
        poly[i] = tmp[i];
    *deg = new_deg;
}

/*  Compute the coefficients φ[0..800] of Φ₁₃₅₃(x) via exact division A/B. */
static void compute_phi(void)
{
    /* Build numerator A = Π (x^d - 1) for d = 3, 11, 41, 1353 */
    size_t degA = 0;
    polyA[0] = 1;
    multiply_by_factor(polyA, &degA, 3);
    multiply_by_factor(polyA, &degA, 11);
    multiply_by_factor(polyA, &degA, 41);
    multiply_by_factor(polyA, &degA, 1353);

    /* Build denominator B = Π (x^d - 1) for d = 1, 33, 123, 451 */
    size_t degB = 0;
    polyB[0] = 1;
    multiply_by_factor(polyB, &degB, 1);
    multiply_by_factor(polyB, &degB, 33);
    multiply_by_factor(polyB, &degB, 123);
    multiply_by_factor(polyB, &degB, 451);

    /* Exact polynomial division: A = B * Φ₁₃₅₃(x)  →  Φ = A / B */
    seq_t rem[DEG_A + 1];
    for (size_t i = 0; i <= DEG_A; ++i)
        rem[i] = polyA[i];

    for (int k = (int)DEG_A; k >= (int)DEG_B; --k) {
        seq_t coeff = rem[k];
        size_t qidx = (size_t)(k - DEG_B);        /* index in the quotient */
        phi[qidx] = coeff;                       /* store coefficient φ[qidx] */

        /* Subtract coeff * B(x) * x^{qidx} from the remainder */
        for (size_t j = 0; j <= DEG_B; ++j)
            rem[qidx + j] -= coeff * polyB[j];
    }
    /* After the loop, rem[0..DEG_B-1] should be all zeros.  φ[0] is constant term. */
}

/*  Compute the first PERIOD coefficients of the reciprocal series
 *  1 / Φ₁₃₅₃(x) using the recurrence
 *       a(0) = 1
 *       a(n) = - Σ_{k=1}^{800} φ(k) * a(n-k)   for n ≥ 1.      */
static void compute_series(void)
{
    seq_series[0] = 1;   /* a(0) = 1 */

    for (size_t n = 1; n < PERIOD; ++n) {
        seq_t sum = 0;
        size_t limit = n < DEG_Q ? n : DEG_Q;
        for (size_t k = 1; k <= limit; ++k)
            sum += phi[k] * seq_series[n - k];
        seq_series[n] = -sum;
    }
}

/*  Public sequence function.  Calls the initialization once. */
seq_t a(idx_t n)
{
    if (!initialized) {
        compute_phi();
        compute_series();
        initialized = 1;
    }
    return seq_series[(size_t)(n % PERIOD)];
}
