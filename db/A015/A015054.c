/*
 * A015054  Inverse of 1045th cyclotomic polynomial.
 * https://oeis.org/A015054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   poly_mult_shift_minus
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A015054 – Inverse of the 1045th cyclotomic polynomial
 *
 *  The coefficients a(n) satisfy
 *      a(0) = 1
 *      a(n) = -∑_{k=1}^{720} c_k · a(n-k)     (n ≥ 1)
 *
 *  where Φ_1045(x) = ∑_{k=0}^{720} c_k x^k is the 1045th cyclotomic
 *  polynomial (c_0 = 1).  The sequence is periodic with period 1045, so
 *  a(n+1045) = a(n) for all n ≥ 0.
 */

#include <stddef.h>   /* for size_t */
#include "../utils/poly_mult_shift_minus.h"

/* --------------------------------------------------------------------
 * Main function returning the nth term of the sequence.
 * -------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    /* The period of the sequence. */
    const idx_t period = 1045;

    /* Static tables are filled once on first call. */
    static idx_t  initialized = 0;
    static seq_t coeff[721];   /* coefficients of Φ_1045(x), 0 … 720 */
    static seq_t A[1045];      /* first 1045 terms a(0)…a(1044) */

    if (!initialized) {
        /* ----------------------------------------------------------------
         * 1. Compute the cyclotomic polynomial Φ_1045(x).
         *    Φ_1045(x) = N(x)/D(x) with
         *      N(x) = (x^5 − 1)(x^11 − 1)(x^19 − 1)(x^1045 − 1)
         *      D(x) = (x − 1)(x^55 − 1)(x^95 − 1)(x^209 − 1)
         * ---------------------------------------------------------------- */

        /* ---------- numerator ------------------------------------------ */
        int64_t num[1081] = {0};   /* size : 1081 = 1 + 5 + 11 + 19 + 1045 */
        int      nlen = 1;
        num[0] = 1;                 /* start with the constant 1 */

        /* multiply by (x^5 – 1) */
        poly_mult_shift_minus(num, &nlen, 5);   /* nlen becomes 6 */
        /* multiply by (x^11 – 1) */
        poly_mult_shift_minus(num, &nlen, 11);  /* nlen becomes 17 */
        /* multiply by (x^19 – 1) */
        poly_mult_shift_minus(num, &nlen, 19);  /* nlen becomes 36 */
        /* multiply by (x^1045 – 1) */
        poly_mult_shift_minus(num, &nlen, 1045);/* nlen becomes 1081 */

        /* ---------- denominator ------------------------------------------ */
        int64_t den[361] = {0};     /* size : 1 + 1 + 55 + 95 + 209 = 361 */
        int      dlen = 1;
        den[0] = 1;                 /* start with the constant 1 */

        /* multiply by (x – 1) */
        poly_mult_shift_minus(den, &dlen, 1);   /* dlen becomes 2 */
        /* multiply by (x^55 – 1) */
        poly_mult_shift_minus(den, &dlen, 55);  /* dlen becomes 57 */
        /* multiply by (x^95 – 1) */
        poly_mult_shift_minus(den, &dlen, 95);  /* dlen becomes 152 */
        /* multiply by (x^209 – 1) */
        poly_mult_shift_minus(den, &dlen, 209); /* dlen becomes 361 */

        /* ---------- exact division N / D -------------------------------- */
        const int degN = nlen - 1;   /* 1080 */
        const int degD = dlen - 1;   /* 360 */
        const int degQ = degN - degD;/* 720, expected degree of Φ_1045 */

        /* remainder buffer used for the long division */
        int64_t rem[1081];
        for (int i = 0; i <= degN; ++i)
            rem[i] = num[i];

        /* perform the division; D has leading coefficient 1 */
        for (int i = degN; i >= degD; --i) {
            int64_t q = rem[i];                /* coefficient of quotient */
            coeff[i - degD] = (seq_t)q;        /* store to output array */

            /* subtract q * D(x) shifted by (i - degD) from the remainder */
            int shift = i - degD;
            for (int j = 0; j <= degD; ++j)
                rem[shift + j] -= q * den[j];
        }

        /* ---------- compute first 1045 terms by recurrence ---------------- */
        A[0] = (seq_t)1;
        for (idx_t m = 1; m < period; ++m) {
            int limit = (m < 720) ? (int)m : 720;
            int64_t sum = 0;                     /* use a wider type
                                                       for intermediate sums   */
            for (int k = 1; k <= limit; ++k)
                sum += (int64_t)coeff[k] * (int64_t)A[m - k];
            A[m] = -(seq_t)sum;                  /* guaranteed to be
                                                       –1, 0 or 1 */
        }

        initialized = 1;
    }

    /* Return the term using periodicity. */
    return A[n % period];
}
