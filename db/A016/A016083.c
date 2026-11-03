/*
 * A016083  Inverse of 2074th cyclotomic polynomial.
 * https://oeis.org/A016083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the nth coefficient of the series expansion of
 *            1/φ_2074(x), the inverse of the 2074th cyclotomic polynomial.
 *            The sequence is periodic with period 2074, so a(n) = a(n mod
 *            2074)
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Validated: standard, pass, 97 terms through n = 2786 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Multiply a polynomial (poly[0..deg]) by (x^k – 1) into out[0..deg+k].
   The result is written into out, which must have room for deg+k+1 coeffs. */
static void mul_xk_minus_one(seq_t *poly, size_t deg, size_t k, seq_t *out)
{
    size_t new_deg = deg + k;
    for (size_t i = 0; i <= new_deg; ++i) out[i] = 0;
    for (size_t i = 0; i <= deg; ++i) {
        out[i + k] += poly[i];
        out[i]     -= poly[i];
    }
}

/* Perform exact long division of polynomial numerator (N) by monic
   denominator (D).  N is modified in place and must be large enough
   to hold the original degree.  The quotient is written into Q.   */
static void poly_divide_exact(seq_t *N, size_t N_deg,
                              seq_t *D, size_t D_deg,
                              seq_t *Q, size_t Q_deg)
{
    for (size_t i = N_deg; i >= D_deg; --i) {
        seq_t coef = N[i];
        size_t q_idx = i - D_deg;
        Q[q_idx] = coef;
        for (size_t j = 0; j <= D_deg; ++j) {
            N[i - j] -= coef * D[D_deg - j];
        }
        if (i == D_deg) break;   /* avoid underflow of size_t */
    }
    /* The remainder in N[0..D_deg-1] should be zero. */
}

#define PERIOD 2074
#define MAX_N 2155        /* degree of the numerator polynomial */
#define MAX_D 1200        /* degree of the denominator polynomial */
#define PHI_DEG 960

static seq_t *a_terms = NULL;    /* periodic sequence, size PERIOD */
static seq_t *phi     = NULL;    /* φ_2074(x) coefficients, size PHI_DEG+1 */
static bool initialized = false;

/* Initialize periodic sequence a_terms and φ coefficients. */
static void init_sequence(void)
{
    static seq_t N[MAX_N];
    static seq_t D[MAX_D];
    static seq_t temp[MAX_N];
    seq_t *N2 = temp;  /* copy of N in long division */

    /* Build numerator N = (x^2074-1)(x^61-1)(x^17-1)(x^2-1) */
    size_t degN = 0;
    N[0] = 1;
    mul_xk_minus_one(N, degN, 2074, temp);
    for (size_t i = 0; i <= degN + 2074; ++i) N[i] = temp[i];
    degN += 2074;

    mul_xk_minus_one(N, degN, 61, temp);
    for (size_t i = 0; i <= degN + 61; ++i) N[i] = temp[i];
    degN += 61;

    mul_xk_minus_one(N, degN, 17, temp);
    for (size_t i = 0; i <= degN + 17; ++i) N[i] = temp[i];
    degN += 17;

    mul_xk_minus_one(N, degN, 2, temp);
    for (size_t i = 0; i <= degN + 2; ++i) N[i] = temp[i];
    degN += 2;   /* degN should now be 2154 */

    /* Build denominator D = (x^1037-1)(x^122-1)(x^34-1)(x-1) */
    size_t degD = 0;
    D[0] = 1;
    mul_xk_minus_one(D, degD, 1037, temp);
    for (size_t i = 0; i <= degD + 1037; ++i) D[i] = temp[i];
    degD += 1037;

    mul_xk_minus_one(D, degD, 122, temp);
    for (size_t i = 0; i <= degD + 122; ++i) D[i] = temp[i];
    degD += 122;

    mul_xk_minus_one(D, degD, 34, temp);
    for (size_t i = 0; i <= degD + 34; ++i) D[i] = temp[i];
    degD += 34;

    mul_xk_minus_one(D, degD, 1, temp);
    for (size_t i = 0; i <= degD + 1; ++i) D[i] = temp[i];
    degD += 1;   /* degD should now be 1194 */

    /* Allocate memory for φ and a_terms */
    phi     = mem_new(seq_t, (PHI_DEG + 1));
    a_terms = mem_new(seq_t, PERIOD);

    /* Exact division: φ = N / D */
    seq_t Q[PHI_DEG + 1];
    for (size_t i = 0; i <= PHI_DEG; ++i) Q[i] = 0;
    poly_divide_exact(N2, degN, D, degD, Q, PHI_DEG);
    for (size_t i = 0; i <= PHI_DEG; ++i) phi[i] = Q[i];

    /* Compute periodic sequence a(k) for k = 0 .. PERIOD-1
       using recurrence: sum_{i=1..min(k,deg)} p[i]*a[k-i] = 0,
       with a[0] = 1. */
    a_terms[0] = 1;
    for (idx_t k = 1; k < PERIOD; ++k) {
        seq_t sum = 0;
        size_t up = (k <= PHI_DEG) ? k : PHI_DEG;
        for (size_t i = 1; i <= up; ++i)
            sum += phi[i] * a_terms[k - i];
        a_terms[k] = -sum;
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_sequence();
    idx_t idx = n % PERIOD;
    return a_terms[idx];
}
