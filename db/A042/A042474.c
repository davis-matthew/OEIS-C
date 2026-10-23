/*
 * A042474  Numerators of continued fraction convergents to sqrt(765).
 * https://oeis.org/A042474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 15, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042474: Numerators of convergents to sqrt(765) */
/* Sequence: a(0)=27, a(1)=28, a(2)=55, ... */
/* Computed via continued–fraction recurrence for sqrt(765). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Square‑free part of D = 765 and its integer part sqrt(D). */
    const seq_t D   = (seq_t)765;
    const seq_t a0  = (seq_t)27;          /* floor(sqrt(765)) */

    if (n == 0) return a0;                /* base case */

    /* States for the continued‑fraction expansion (m,d,a). */
    seq_t m_prev = (seq_t)0;
    seq_t d_prev = (seq_t)1;
    seq_t a_prev = a0;

    /* Numerator recurrence: p_-1 = 1, p_0 = a0. */
    seq_t p_prev2 = (seq_t)1;  /* p_{k-2} */
    seq_t p_prev1 = a0;        /* p_{k-1} */
    seq_t p = a0;              /* current numerator (initially p_0) */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Next partial quotient a_k. */
        seq_t m = d_prev * a_prev - m_prev;
        seq_t d = (D - m * m) / d_prev;
        seq_t a = (a0 + m) / d;      /* floor division */

        /* Update the numerator: p_k = a_k * p_{k-1} + p_{k-2}. */
        p = a * p_prev1 + p_prev2;

        /* Shift previous terms for next iteration. */
        p_prev2 = p_prev1;
        p_prev1 = p;

        m_prev = m;
        d_prev = d;
        a_prev = a;
    }

    return p;   /* returns the numerator of the n‑th convergent */
}
