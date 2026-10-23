/*
 * A014548  Inverse of 539th cyclotomic polynomial.
 * https://oeis.org/A014548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the power series 1 / Φ_539(x)
 *            modulo x^539-1. Equivalently, (∑_{i=0}^{420} c_i x^i) · (∑_{k≥0}
 *            a(k) x^k) ≡ 1 (mod x^539-1)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth term of OEIS sequence A014548,
   the inverse of the 539th cyclotomic polynomial.
   The sequence is periodic with period 539. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Size of the auxiliary arrays used in polynomial arithmetic. */
#define MAX_DEG 600
#define PERIOD 539

/* Helper: multiply two polynomials A and B into Res.
   All polynomials are represented by arrays of type seq_t,
   with the first element being the constant term.  The
   highest indices are their respective degrees.  The size
   ResLen must be at least Alen+Blw-1. */
static void poly_mul(const seq_t *A, size_t Alen,
                     const seq_t *B, size_t Blen,
                     seq_t *Res, size_t ResLen) {
    for (size_t i = 0; i < ResLen; ++i) Res[i] = 0;
    for (size_t i = 0; i < Alen; ++i) {
        if (A[i] == 0) continue;
        for (size_t j = 0; j < Blen; ++j) {
            if (B[j] == 0) continue;
            Res[i + j] += A[i] * B[j];
        }
    }
}

/* Helper: divide 'Dividend' by the monic divisor 'Divisor'.
   The divisor must have leading coefficient 1 (monic).
   The remainder is assumed to be zero.  QuotLen = Dlen-Mlen+1. */
static void poly_div_monic(const seq_t *Dividend, size_t Dlen,
                           const seq_t *Divisor, size_t Mlen,
                           seq_t *Quot, size_t Qlen) {
    /* Temp buffer uses seq_t type; copy by assignment. */
    seq_t temp[MAX_DEG];
    for (size_t i = 0; i < Dlen; ++i) temp[i] = Dividend[i];
    for (size_t i = 0; i < Qlen; ++i) Quot[i] = 0;
    /* Work from high degree downwards. */
    for (long i = (long)Dlen - 1; i >= (long)Mlen - 1; --i) {
        seq_t coeff = temp[i];
        if (coeff == 0) continue;
        size_t qidx = i - (Mlen - 1);
        Quot[qidx] = coeff;
        /* Subtract coeff * Divisor from temp segment. */
        for (size_t j = 0; j < Mlen; ++j) {
            temp[qidx + j] -= coeff * Divisor[j];
        }
    }
}

/* Compute the coefficients of Φ_539(x) once and store them in
   a static array.  Then compute the first 539 terms of the
   desired sequence by the recurrence described above. */
static void compute_period(seq_t *period) {
    /* 1. Construct Φ_539(x) = (x^539-1)*(x^7-1)/[(x^77-1)*(x^49-1)] */
    seq_t poly1[MAX_DEG] = {0}, poly2[MAX_DEG] = {0},
          poly3[MAX_DEG] = {0}, poly4[MAX_DEG] = {0};
    seq_t win7[MAX_DEG] = {0}, div77[MAX_DEG] = {0},
          div49[MAX_DEG] = {0};

    /* (x^539-1) */
    poly1[0] = (seq_t)-1;
    poly1[539] = (seq_t)1;
    size_t len1 = 540;

    /* (x^7-1) */
    win7[0] = (seq_t)-1;
    win7[7] = (seq_t)1;
    size_t lenwin7 = 8;

    /* Multiply to get poly2. */
    poly_mul(poly1, len1, win7, lenwin7, poly2, len1 + lenwin7 - 1);
    size_t len2 = len1 + lenwin7 - 1;      /* 547 */

    /* Divide by (x^77-1). */
    div77[0] = (seq_t)-1;
    div77[77] = (seq_t)1;
    size_t len77 = 78;
    poly_div_monic(poly2, len2,
                   div77, len77,
                   poly3, len2 - len77 + 1);
    size_t len3 = len2 - len77 + 1;        /* 470 */

    /* Divide by (x^49-1). */
    div49[0] = (seq_t)-1;
    div49[49] = (seq_t)1;
    size_t len49 = 50;
    poly_div_monic(poly3, len3,
                   div49, len49,
                   poly4, len3 - len49 + 1);
    size_t len4 = len3 - len49 + 1;        /* 421 */

    /* 2. Retrieve the coefficients of Φ_539(x).  They are
       coefficients c[0] ... c[420] with c[0] == 1. */
    seq_t c[421];
    for (size_t i = 0; i < len4; ++i) c[i] = poly4[i];

    /* 3. Compute the period of a(n) using the recurrence
       a[0] = 1,
       for n >= 1: a[n] = -sum_{i=1}^{min(n,420)} c[i] * a[n-i]. */
    period[0] = (seq_t)1;
    for (size_t n = 1; n < PERIOD; ++n) {
        seq_t sum = 0;
        size_t maxi = n < 420 ? n : 420;
        for (size_t i = 1; i <= maxi; ++i) {
            sum += c[i] * period[n - i];
        }
        period[n] = -sum;
    }
}

/* Static storage for the computed period.  The flag ensures that
   compute_period is called at most once, even if a() is called
   non‑monotonically. */
static seq_t period[PERIOD];
static int period_initialized = 0;

/* Public entry point requested in the problem statement. */
seq_t a(idx_t n) {
    if (!period_initialized) {
        compute_period(period);
        period_initialized = 1;
    }
    return period[(size_t)(n % PERIOD)];
}
