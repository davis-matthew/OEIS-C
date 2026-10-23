/*
 * A132837  Largest terms a(n) forming a self-convolution 4th power of an
 *          integer sequence (A132838) such that: a(n) <= 4*a(n-1) for n>0
 *          with a(0)=1.
 * https://oeis.org/A132837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Largest terms a(n) forming a self-convolution 4th power of an
 *            integer sequence (A132838) such that a(n) <= 4*a(n-1) for n>0,
 *            with a(0)=1. The implementation follows the constructive
 *            recurrence: a[n] = L[n] + 4*b[n] where L[n] is the coefficient
 *            of degree n in B(x)^4 computed with only the coeffic...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static idx_t max_computed = (idx_t)(-1);
static size_t cap = 0;                       /* current array capacity   */
static seq_t *b = NULL;          /* coefficients of B(x)         */
static seq_t *a_vals = NULL;     /* a(n) values                   */
static seq_t *P2_arr = NULL;     /* B(x)^2 coefficients           */
static seq_t *P3_arr = NULL;     /* B(x)^3 coefficients           */
static seq_t *a_coeff = NULL;    /* B(x)^4 coefficients (convolution) */

/* Resize internal arrays so that indices up to need_n are valid. */
static void ensure_capacity(size_t need_n)
{
    if (need_n <= cap) return;

    size_t newcap = (cap ? cap << 1 : 1);
    while (newcap <= need_n) newcap <<= 1;

    b          = (seq_t*)realloc(b,          (newcap+1) * sizeof(seq_t));
    a_vals     = (seq_t*)realloc(a_vals,     (newcap+1) * sizeof(seq_t));
    P2_arr     = (seq_t*)realloc(P2_arr,     (2*newcap+1) * sizeof(seq_t));
    P3_arr     = (seq_t*)realloc(P3_arr,     (3*newcap+1) * sizeof(seq_t));
    a_coeff    = (seq_t*)realloc(a_coeff,    (4*newcap+1) * sizeof(seq_t));

    /* initialise newly allocated slots */
    for (size_t i = cap+1; i <= newcap; ++i) {
        b[i]       = 0;
        a_vals[i]  = 0;
    }
    for (size_t i = 2*cap+1; i <= 2*newcap; ++i) P2_arr[i] = 0;
    for (size_t i = 3*cap+1; i <= 3*newcap; ++i) P3_arr[i] = 0;
    for (size_t i = 4*cap+1; i <= 4*newcap; ++i) a_coeff[i] = 0;

    /* initialise first coefficient if we are starting from scratch */
    if (cap == 0) {
        b[0] = (seq_t)1;
        a_vals[0] = (seq_t)1;
        P2_arr[0] = (seq_t)1;
        P3_arr[0] = (seq_t)1;
        a_coeff[0] = (seq_t)1;
    }

    cap = newcap;
}

/* public function ------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* initialise on first call    */
    if (max_computed == (idx_t)(-1)) {
        ensure_capacity((size_t)n);
        max_computed = 0;   /* base terms already set in ensure_capacity   */
    }

    /* compute missing values up to n   */
    if ((idx_t)max_computed < n) {
        ensure_capacity((size_t)n);
        seq_t a_prev = a_vals[max_computed];   /* a(max_computed)      */

        for (idx_t idx = (idx_t)max_computed + 1; idx <= n; ++idx) {
            size_t i = (size_t)idx;
            seq_t L = a_coeff[i];        /* coefficient with old B           */

            /* choose the largest possible b[i] keeping a[i] <= 4*a_prev   */
            seq_t four_a_prev = (seq_t)4 * a_prev;
            seq_t max_b;
            if (four_a_prev <= L) {
                max_b = 0;
            } else {
                seq_t diff = four_a_prev - L;
                max_b = diff / (seq_t)4;
            }

            seq_t d = max_b;             /* new coefficient b[i]            */
            b[i] = d;

            /* a[i] = L + 4*d                                            */
            seq_t A_i = L + (seq_t)4 * d;
            a_vals[i] = A_i;
            a_prev = A_i;

            /* pre‑compute powers of d for later use                    */
            seq_t d2 = d * d;
            seq_t d3 = d2 * d;
            seq_t d4 = d3 * d;
            seq_t four_d  = (seq_t)4 * d;
            seq_t six_d2  = (seq_t)6 * d2;
            seq_t four_d3 = (seq_t)4 * d3;

            /* ----------------------------------------------------------------
               Update the convolution array a_coeff (B^4) using the old P3, P2 and b.
               ---------------------------------------------------------------- */
            /* term 4*d * P3 shifted by i                                   */
            for (size_t j = 0; j <= 3*(i-1); ++j) {
                seq_t add = four_d * P3_arr[j];
                if (add != 0) a_coeff[j + i] += add;
            }
            /* term 6*d^2 * P2 shifted by 2*i                               */
            for (size_t j = 0; j <= 2*(i-1); ++j) {
                seq_t add = six_d2 * P2_arr[j];
                if (add != 0) a_coeff[j + 2*i] += add;
            }
            /* term 4*d^3 * b shifted by 3*i                                 */
            for (size_t j = 0; j < i; ++j) {
                seq_t add = four_d3 * b[j];
                if (add != 0) a_coeff[j + 3*i] += add;
            }
            /* term d^4 at 4*i                                               */
            if (d4 != 0) a_coeff[4*i] += d4;

            /* ----------------------------------------------------------------
               Update the auxiliary arrays P2 (B^2) and P3 (B^3) using only
               the old values of b, P2, and P3.
               ---------------------------------------------------------------- */
            /* update P2:  B^2 new = B^2 old + 2*d * shift(b, i) + d^2 at 2*i        */
            for (size_t j = 0; j < i; ++j) {
                seq_t prod = (seq_t)2 * d * b[j];
                if (prod != 0) P2_arr[j + i] += prod;
            }
            if (d2 != 0) P2_arr[2*i] += d2;

            /* update P3:  B^3 new = B^3 old + 3*d * shift(P2 old, i)
                         + 3*d^2 * shift(b, 2*i) + d^3 at 3*i                  */
            for (size_t j = 0; j <= 2*(i-1); ++j) {
                seq_t prod = (seq_t)3 * d * P2_arr[j];
                if (prod != 0) P3_arr[j + i] += prod;
            }
            for (size_t j = 0; j < i; ++j) {
                seq_t prod = (seq_t)3 * d2 * b[j];
                if (prod != 0) P3_arr[j + 2*i] += prod;
            }
            if (d3 != 0) P3_arr[3*i] += d3;
        }
        max_computed = n;
    }
    return a_vals[n];
}
