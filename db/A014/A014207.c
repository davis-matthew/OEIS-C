/*
 * A014207  Inverse of 198th cyclotomic polynomial.
 * https://oeis.org/A014207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   poly_mul_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A014207 – coefficients of 1/Φ_198(x).
   The coefficients are periodic with period 198.
   They are obtained from the recurrence
       a(0) = 1,
       a(n) = -∑_{k=1}^{60} φ_k * a(n-k)   (n>0),
   where φ_k are the coefficients of the cyclotomic polynomial Φ_198(x). */

#include <stddef.h>
#include <stdint.h>
#include "../utils/poly_mul_factor.h"

/* -------------------------------------------------------------------------
   Exact division of polynomial N(x) by D(x); N and D are modified,
   Q receives the quotient.  D's leading coefficient is assumed 1.
   -------------------------------------------------------------- */
static void poly_divide(seq_t *N, int degN,
                        const seq_t *D, int degD,
                        seq_t *Q)
{
    int degQ = degN - degD;
    for (int i = 0; i <= degQ; ++i) Q[i] = 0;

    for (int i = degN; i >= degD; --i) {
        seq_t coeff = N[i];               /* coefficient of x^i in N   */
        Q[i - degD] = coeff;              /* quotient term            */
        for (int j = 0; j <= degD; ++j) { /* subtract coeff*D from N    */
            N[i - degD + j] -= coeff * D[j];
        }
    }
}

/* -------------------------------------------------------------------------
   Compute the 61 coefficients φ_0 … φ_60 of the cyclotomic polynomial
   Φ_198(x) = (x^198-1)(x^33-1)(x^9-1)(x^6-1) / ((x^99-1)(x^66-1)(x^18-1)(x^3-1))
   Result is stored in phi[0..60].
   -------------------------------------------------------------- */
static void compute_phi_coefficients(seq_t phi[61])
{
    seq_t num[260] = {0};
    seq_t denom[260] = {0};
    int deg_num = 0, deg_denom = 0;

    num[0] = 1;
    denom[0] = 1;

    /* numerator factors */
    poly_mul_factor(num, &deg_num, 198);
    poly_mul_factor(num, &deg_num, 33);
    poly_mul_factor(num, &deg_num, 9);
    poly_mul_factor(num, &deg_num, 6);

    /* denominator factors */
    poly_mul_factor(denom, &deg_denom, 99);
    poly_mul_factor(denom, &deg_denom, 66);
    poly_mul_factor(denom, &deg_denom, 18);
    poly_mul_factor(denom, &deg_denom, 3);

    /* Perform exact division N / D to obtain Φ_198(x) */
    seq_t num_copy[260];
    for (int i = 0; i <= deg_num; ++i) num_copy[i] = num[i];
    poly_divide(num_copy, deg_num, denom, deg_denom, phi);
}

/* -------------------------------------------------------------------------
   Static storage for the 198-periodic values.
   -------------------------------------------------------------- */
static seq_t period[198];
static int period_initialized = 0;

/* -------------------------------------------------------------------------
   Return the n-th term of the sequence.
   -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!period_initialized) {
        /* Compute the cyclotomic polynomial coefficients once. */
        seq_t phi[61];
        compute_phi_coefficients(phi);

        /* Generate 198-periodic terms using the recurrence. */
        period[0] = 1;
        for (int i = 1; i < 198; ++i) {
            seq_t sum = 0;
            int limit = (i < 61) ? i : 60;   /* φ_k exists only up to k=60 */
            for (int k = 1; k <= limit; ++k) {
                sum -= phi[k] * period[i - k];
            }
            period[i] = sum;
        }
        period_initialized = 1;
    }

    return period[(idx_t)(n % 198)];
}
