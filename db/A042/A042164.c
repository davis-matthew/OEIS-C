/*
 * A042164  Numerators of continued fraction convergents to sqrt(607).
 * https://oeis.org/A042164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042164: Numerators of continued‑fraction convergents to sqrt(607)
   The numerators p_k satisfy
   p_{-2} = 0,  p_{-1} = 1
   p_k   = a_k * p_{k-1} + p_{k-2}      (k ≥ 0)
   where a_0 = ⌊√607⌋ = 24 and the sequence (a_k)_{k≥1}
   is the periodic part of the continued fraction of √607.
   This function computes p_n = a(n).                                           */

#include <stdlib.h>
#include <stddef.h>

static const idx_t A0 = 24;                /* floor(sqrt(607)) */

/* Periodic coefficients of the continued fraction of sqrt(607). */
static idx_t period_len = 0;
static idx_t *period_coeffs = NULL;

/* Compute the periodic part once.  Uses only idx_t arithmetic. */
static void init_period(void)
{
    if (period_len != 0)            /* already computed */
        return;

    idx_t m = 0;
    idx_t d = 1;
    idx_t a = A0;
    size_t cap = 8;                 /* initial capacity of array */

    period_coeffs = (idx_t *)malloc(cap * sizeof(idx_t));
    while (period_coeffs == NULL) { /* defensive test – should never fail */
        /* extend array if needed */
        cap <<= 1;
        period_coeffs = (idx_t *)realloc(period_coeffs, cap * sizeof(idx_t));
    }

    for (;;) {
        /* m = d*a - m */
        m = d * a - m;
        /* d = (D - m*m) / d  ,   D = 607 */
        d = (607 - m * m) / d;
        /* a = (A0 + m) / d */
        a = (A0 + m) / d;

        if (period_len >= cap) {    /* grow array if necessary */
            cap <<= 1;
            period_coeffs = (idx_t *)realloc(period_coeffs, cap * sizeof(idx_t));
        }
        period_coeffs[period_len++] = a;   /* store a_k for k≥1 */

        /* The period ends when a == 2*A0 */
        if (a == 2 * A0)
            break;
    }
}

/* Numerator of the n‑th convergent to sqrt(607). */
seq_t a(idx_t n)
{
    init_period();                 /* make sure the period is ready */

    /* Recurrence: p_{-2}=0, p_{-1}=1, p_k = coeff_k*p_{k-1}+p_{k-2} */
    seq_t p_prev2 = 0;             /* p_{-2} */
    seq_t p_prev1 = 1;             /* p_{-1} */
    seq_t p_curr  = 0;

    for (idx_t k = 0; k <= n; ++k) {
        idx_t coeff;
        if (k == 0)                /* a_0 */
            coeff = A0;
        else                       /* periodic part: a_k (k≥1) */
            coeff = period_coeffs[(k - 1) % period_len];

        /* p_k = coeff * p_{k-1} + p_{k-2}   */
        p_curr = (seq_t)coeff * p_prev1 + p_prev2;

        /* shift for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }

    return p_curr;                 /* this is a(n) = p_n */
}
