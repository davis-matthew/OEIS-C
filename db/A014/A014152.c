/*
 * A014152  Inverse of 143rd cyclotomic polynomial.
 * https://oeis.org/A014152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the n-th coefficient of 1 / Φ_143(x), where Φ_143 is the
 *            143rd cyclotomic polynomial
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is periodic with period 143. */

#include <stddef.h>
#include <stdint.h>

static void poly_mul(const seq_t *a, size_t degA,
                     const seq_t *b, size_t degB,
                     seq_t *out)
{
    /* out coefficient array must be large enough to hold degA+degB */
    size_t i, j;
    for (i = 0; i <= degA + degB; ++i)
        out[i] = 0;
    for (i = 0; i <= degA; ++i) {
        if (a[i] == 0) continue;
        for (j = 0; j <= degB; ++j) {
            if (b[j] == 0) continue;
            out[i + j] += a[i] * b[j];
        }
    }
}

static void compute_period(seq_t period[143])
{
    /* Build Φ_143(x) = (x^143 - 1) / ((x-1)*(x^10+…+1)*(x^12+…+1)) */
    /* Step 1: Build polynomials (x-1), (x^10+…+1), (x^12+…+1) */
    seq_t p1[2] = { -1, 1 };         /* degree 1 */
    seq_t p2[11];
    seq_t p3[13];
    size_t i;
    for (i = 0; i <= 10; ++i) p2[i] = 1;   /* degree 10 */
    for (i = 0; i <= 12; ++i) p3[i] = 1;   /* degree 12 */

    /* Step 2: Multiply polynomials */
    seq_t tmp1[12];   /* degrees 0..11  (1+10) */
    poly_mul(p1, 1, p2, 10, tmp1);

    seq_t phi[24];    /* degrees 0..23  (11+12) */
    poly_mul(tmp1, 11, p3, 12, phi);

    /* Step 3: Prepare N(x) = x^143 - 1 (degree 143) */
    seq_t rem[144];
    for (i = 0; i < 144; ++i)
        rem[i] = 0;
    rem[0]   = -1;
    rem[143] = 1;

    /* Step 4: Long division rem / phi to obtain Φ_143 coefficients */
    seq_t q[121];          /* degree 120 */
    for (i = 0; i <= 120; ++i)
        q[i] = 0;
    /* phi[23] is leading coefficient (should be 1) */
    for (i = 143; i >= 23; --i) {
        seq_t coeff = rem[i];
        q[i - 23] = coeff;
        if (coeff != 0) {
            size_t j;
            for (j = 0; j <= 23; ++j) {
                rem[i - 23 + j] -= coeff * phi[j];
            }
        }
    }
    /* After division, rem should be all zeros (remainder). */

    /* Step 5: Use recurrence a_k = - Σ_{i=1}^{min(k,120)} c_i * a_{k-i} */
    seq_t a[143];
    a[0] = 1;            /* Φ_143 has constant term 1, so a_0 = 1 */
    for (i = 1; i < 143; ++i) {
        seq_t sum = 0;
        size_t max_i = (i <= 120) ? i : 120;
        size_t j;
        for (j = 1; j <= max_i; ++j) {
            sum += q[j] * a[i - j];
        }
        a[i] = -sum;
    }

    /* Copy to output period array */
    for (i = 0; i < 143; ++i)
        period[i] = a[i];
}

seq_t a(idx_t n)
{
    static seq_t period[143] = {0};
    static int initialized = 0;
    if (!initialized) {
        compute_period(period);
        initialized = 1;
    }
    return period[n % 143];
}
