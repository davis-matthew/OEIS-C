/*
 * A015652  Inverse of 1643rd cyclotomic polynomial.
 * https://oeis.org/A015652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: coefficients a(n) of the expansion 1/Φ₁₆₄₃(x)
 * The function computes a(n) as the coefficients of the formal power
 * series inverse of the 1643rd cyclotomic polynomial.  The polynomial
 * Φ₁₆₄₃(x) has degree φ(1643)=1560 and coefficients in {–1,0,1}.
 * The sequence satisfies the linear recurrence
 *
 *     a(0) = 1,
 *     a(n) = -∑_{k=1}^{1560} c[k] * a(n-k)   (n ≥ 1),
 *
 * where c[1…1560] are the coefficients of Φ₁₆₄₃(x).  The sequence is
 * periodic with period 1643; the first 24 terms are all equal to 1.
 */

#include <stddef.h>   /* for size_t, if needed */

#define PERIOD 1643
#define DEG     1560   /* φ(1643) */

/* compute the coefficients of Φ_1643(x) using the formula
 * Φ_pq(x) = Φ_p(x^q) / Φ_p(x),   p=31, q=53.  Result is stored
 * in the static array coeff: coeff[0] is the constant term (1),
 * coeff[1]…coeff[DEG] are the remaining coefficients. */
static void compute_phi_coeffs(int *coeff)
{
    /* Temporary numerator polynomial: Φ_31(x^53) = Σ_{i=0}^{30} x^(53*i) */
    int N[1591] = {0};           /* degrees 0 … 1590 inclusive */
    for (int i = 0; i <= 30; ++i)
        N[53 * i] = 1;

    /* Denominator polynomial Φ_31(x) = 1 + x + … + x^30 (coefficients all 1) */
    /* Polynomial long division N / D  */
    for (int i = 1590; i >= 30; --i) {
        int q = N[i];                 /* coefficient for x^(i-30) in quotient */
        coeff[i - 30] = q;            /* store it */
        if (q != 0) {
            for (int j = 0; j <= 30; ++j)  /* subtract q * x^(i-30+j) */
                N[i - 30 + j] -= q;
        }
    }
    /* After division remainder should be zero; coeff[0] is left in N[0] */
    coeff[0] = N[0];   /* constant term of Φ_1643, always 1 */
}

/* compute the first PERIOD terms of the sequence a(n) and cache them */
static void compute_sequence(seq_t *seq)
{
    static int coeff[DEG + 1];
    static int coeff_init = 0;
    if (!coeff_init) {
        compute_phi_coeffs(coeff);
        coeff_init = 1;
    }

    seq[0] = (seq_t)1;          /* a(0) = 1 */

    for (idx_t n = 1; n < PERIOD; ++n) {
        seq_t sum = 0;
        /* sum = -∑_{k=1}^{DEG} coeff[k] * a(n-k) */
        for (int k = 1; k <= DEG && k <= n; ++k) {
            if (coeff[k] != 0)
                sum -= (seq_t)coeff[k] * seq[n - k];
        }
        seq[n] = sum;
    }
}

seq_t a(idx_t n)
{
    static seq_t seq[PERIOD];
    static int seq_init = 0;
    if (!seq_init) {
        compute_sequence(seq);
        seq_init = 1;
    }
    return seq[n % PERIOD];
}
