/*
 * A042634  Numerators of continued fraction convergents to sqrt(847).
 * https://oeis.org/A042634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(847)   */
/* a(n) is the numerator of the n-th convergent of sqrt(847). */
/* The algorithm uses the standard recurrence for convergents:*/
/*   a_0 = floor(sqrt(847)) = 29,                              */
/*   h_-2 = 0, h_-1 = 1,                                        */
/*   h_n   = a_n * h_{n-1} + h_{n-2}                            */
/* Partial quotients a_n are produced by the periodic NCF   */
/*   algorithm with D = 847.                                   */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Leading partial quotient */
    const seq_t a0 = 29;          /* floor(sqrt(847)) */
    if (n == 0) return a0;

    /* Convergent numerators: h_-2, h_-1, h_n */
    seq_t h_prev2 = 0;            /* h_{-2} */
    seq_t h_prev1 = 1;            /* h_{-1} */
    seq_t h_curr;                 /* h_n */

    /* Continued fraction state for sqrt(847) */
    seq_t m = 0;                  /* m_k */
    seq_t d = 1;                  /* d_k */
    seq_t a_curr = a0;            /* a_k */

    /* Compute h_0 */
    h_curr = a_curr * h_prev1 + h_prev2;   /* = a0 */
    h_prev2 = h_prev1;
    h_prev1 = h_curr;

    /* Generate convergents up to index n */
    for (idx_t k = 1; k <= n; ++k) {
        /* Next continued fraction terms */
        seq_t m_next = d * a_curr - m;
        seq_t d_next = (847 - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        /* Advance state */
        m = m_next;
        d = d_next;
        a_curr = a_next;

        /* Next numerator */
        h_curr = a_curr * h_prev1 + h_prev2;

        /* Shift for next iteration */
        h_prev2 = h_prev1;
        h_prev1 = h_curr;
    }

    return h_curr;
}
