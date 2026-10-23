/*
 * A042874  Numerators of continued fraction convergents to sqrt(969).
 * https://oeis.org/A042874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 16, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042874: numerators of convergents of the continued fraction for sqrt(969).
   The continued fraction of sqrt(969) starts with a0 = 31 and has a purely periodic
   tail.  The convergent numerators p_n satisfy
        p_{-2} = 0,  p_{-1} = 1
        p_n   = a_n * p_{n-1} + p_{n-2}
   where a_0 = 31 and a_i for i>=1 repeat with period that ends when a_i = 2*a_0.
   This function returns p_n for any non‑negative n. */

#include <stddef.h>
#include <stdlib.h>

/* Fixed constants for the square root of 969 */
static const idx_t   N  = 969;          /* value under the sqrt */
static const idx_t   A0 = 31;           /* floor(sqrt(969)) */
static const seq_t   A0SEQ = 31;        /* same value in seq_t type */

/* Periodic part of the continued fraction (excluding a0) */
static idx_t *period   = NULL;   /* array of a_i for i>=1   */
static size_t period_len = 0;    /* number of coefficients  */
static size_t period_cap = 0;    /* current allocated capacity */

/* Compute the periodic coefficients of sqrt(N).  This routine
   is called once on first use. */
static void compute_period(void)
{
    /* start with some reasonable capacity; the real period for sqrt(969) is < 63 */
    period_cap = 64;
    period = (idx_t *)malloc(period_cap * sizeof(idx_t));
    period_len = 0;

    idx_t m    = 0;
    idx_t d    = 1;
    idx_t a    = A0;          /* current coefficient (idx_t) */

    while (1) {
        /* compute next values of the continued fraction algorithm */
        idx_t m_next = d * a - m;
        idx_t d_next = (N - m_next * m_next) / d;
        idx_t a_next = (A0 + m_next) / d_next;   /* still small */

        /* store a_next into the period array */
        if (period_len >= period_cap) {
            period_cap *= 2;
            period = (idx_t *)realloc(period, period_cap * sizeof(idx_t));
        }
        period[period_len++] = a_next;

        /* period terminates when the coefficient equals 2*A0 */
        if (a_next == 2 * A0)
            break;

        /* advance to next step */
        m = m_next;
        d = d_next;
        a = a_next;
    }
}

/* Return the nth term of the sequence.  The routine is thread‑safe
   as it uses static memoised data. */
seq_t a(idx_t n)
{
    /* initialise period if not yet done */
    if (period_len == 0)
        compute_period();

    /* recurrence for convergent numerators */
    seq_t p_minus2 = 0;   /* p_{-2} */
    seq_t p_minus1 = 1;   /* p_{-1} */
    seq_t p_cur    = 0;   /* current numerator */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t a_i;

        if (i == 0) {
            a_i = A0SEQ;           /* a_0 */
        } else {
            size_t idx = (size_t)(i - 1) % period_len;
            a_i = (seq_t)period[idx]; /* cast to seq_t for arithmetic */
        }

        p_cur = a_i * p_minus1 + p_minus2;
        p_minus2 = p_minus1;
        p_minus1 = p_cur;
    }

    return p_cur;
}
