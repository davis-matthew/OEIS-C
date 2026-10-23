/*
 * A016076  Inverse of 2067th cyclotomic polynomial.
 * https://oeis.org/A016076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the formal power series
 *            1/Φ_{2067}(x), where Φ_{2067}(x) is the 2067th cyclotomic
 *            polynomial. The sequence is periodic with period 2067. The
 *            routine computes the period once and returns the requested term
 *            by indexing into the cached array
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Helper: polynomial long division of a monic divisor.
   dividend: coefficients (low → high) of dividend, degree dividend_deg
   divisor:  coefficients of divisor, degree divisor_deg
   quotient: output array of length dividend_deg - divisor_deg + 1   */
static void poly_divide(const seq_t *dividend, size_t dividend_deg,
                        const seq_t *divisor, size_t divisor_deg,
                        seq_t *quotient)
{
    /* Work on a copy of the dividend */
    seq_t *rem = (seq_t *)calloc(dividend_deg + 1, sizeof(seq_t));
    for (size_t i = 0; i <= dividend_deg; ++i)
        rem[i] = dividend[i];

    /* Perform long division */
    for (int i = (int)dividend_deg; i >= (int)divisor_deg; --i) {
        seq_t coeff = rem[i];
        if (coeff == 0)
            continue;
        size_t shift = (size_t)(i - divisor_deg);
        quotient[shift] = coeff;
        for (size_t j = 0; j <= divisor_deg; ++j) {
            rem[shift + j] -= coeff * divisor[j];
        }
    }
    /* Remainder is expected to be zero; it is discarded. */
    free(rem);
}

/* Compute the period coefficients and store them in the supplied array. */
static void compute_period(seq_t *period)
{
    const idx_t N = 2067;                 /* n in Φ_n */
    const size_t PERIOD_LEN = 2067;        /* periodicity of the sequence */

    /* 1. List all divisors of N (there are only 8). */
    idx_t divisors[32];
    size_t div_cnt = 0;
    for (idx_t d = 1; d <= N; ++d)
        if (N % d == 0)
            divisors[div_cnt++] = d;

    /* 2. Struct to hold a polynomial: degree and coefficient array. */
    struct Poly { size_t deg; seq_t *coeff; };
    struct Poly *polys = (struct Poly *)calloc(div_cnt, sizeof(struct Poly));

    /* 3. Compute each cyclotomic polynomial Φ_d for d | N. */
    for (size_t di = 0; di < div_cnt; ++di) {
        idx_t d = divisors[di];

        /* Start with x^d - 1 */
        size_t poly_deg = (size_t)d;
        seq_t *poly = (seq_t *)calloc(poly_deg + 1, sizeof(seq_t));
        poly[0] = (seq_t)(-1);        /* constant term -1 */
        poly[poly_deg] = (seq_t)1;    /* coefficient of x^d is +1 */

        /* Divide by all Φ_e for proper divisors e of d. */
        for (size_t ei = 0; ei < di; ++ei) {
            idx_t e = divisors[ei];
            if (d % e != 0)
                continue;

            /* Find index of Φ_e in the polys array */
            size_t ej = 0;
            while (ej < div_cnt && divisors[ej] != e)
                ++ej;

            struct Poly *div_poly = &polys[ej];

            size_t q_deg = poly_deg - div_poly->deg;
            seq_t *q = (seq_t *)calloc(q_deg + 1, sizeof(seq_t));
            poly_divide(poly, poly_deg, div_poly->coeff, div_poly->deg, q);
            free(poly);
            poly = q;
            poly_deg = q_deg;
        }

        polys[di].deg = poly_deg;
        polys[di].coeff = poly;
    }

    /* 4. Φ_N is the desired cyclotomic polynomial. */
    /* Find its index in divisors (it is the last one). */
    const idx_t N_idx = div_cnt - 1;
    struct Poly *phi = &polys[N_idx];

    /* 5. Build the recurrence coefficients: φ_k for k>=1. */
    seq_t *S = (seq_t *)calloc(PERIOD_LEN, sizeof(seq_t));
    S[0] = (seq_t)1;                     /* constant term of Φ_N */
    size_t max_k = phi->deg < (PERIOD_LEN - 1) ? phi->deg : (PERIOD_LEN - 1);
    for (size_t k = 1; k <= max_k; ++k)
        S[k] = phi->coeff[k];

    /* 6. Compute a[0..PERIOD_LEN-1] using the linear recurrence. */
    seq_t *a = (seq_t *)calloc(PERIOD_LEN, sizeof(seq_t));
    a[0] = (seq_t)1;
    for (size_t n = 1; n < PERIOD_LEN; ++n) {
        seq_t sum = 0;
        for (size_t k = 1; k <= n; ++k) {
            sum += S[k] * a[n - k];
        }
        a[n] = -sum;
    }

    /* 7. Copy the period into the output array. */
    for (size_t i = 0; i < PERIOD_LEN; ++i)
        period[i] = a[i];

    /* 8. Free temporaries. */
    free(a);
    free(S);
    for (size_t i = 0; i < div_cnt; ++i)
        free(polys[i].coeff);
    free(polys);
}

/* External function required by the OEIS framework. */
seq_t a(idx_t n)
{
    static seq_t *period = NULL;
    const size_t PERIOD_LEN = 2067;

    if (period == NULL) {
        period = (seq_t *)calloc(PERIOD_LEN, sizeof(seq_t));
        compute_period(period);
    }

    return period[n % PERIOD_LEN];
}
