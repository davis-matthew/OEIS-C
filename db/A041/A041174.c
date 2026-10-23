/*
 * A041174  Numerators of continued fraction convergents to sqrt(97).
 * https://oeis.org/A041174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 23, u64 n <= 52, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041174: numerators of continued fraction convergents of sqrt(97). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case: the first term corresponds to the integer part of sqrt(97). */
    const seq_t a0 = 9;          /* floor(sqrt(97)) */

    /* For n = 0 we simply return the integer part. */
    if (n == 0) return a0;

    /* p[-1] = 1, p[0] = a0  (numerators of convergents) */
    seq_t p_prev2 = 1;          /* p_{k-2} */
    seq_t p_prev  = a0;         /* p_{k-1} */
    seq_t p_curr  = 0;          /* p_k (to be computed) */

    /* Continued fraction construction variables */
    seq_t m = 0;                /* m_k */
    seq_t d = 1;                /* d_k */
    seq_t a_k = a0;             /* a_k (current continued fraction term) */

    idx_t k = 1;                /* We have already handled a_0 */
    while (k <= n)
    {
        /* Compute next continued fraction term a_{k} */
        seq_t m_next = d * a_k - m;
        seq_t d_next = (97 - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;  /* floor division */

        /* Compute next numerator p_k = a_k * p_{k-1} + p_{k-2} */
        p_curr = a_next * p_prev + p_prev2;

        /* Shift for next iteration */
        p_prev2 = p_prev;
        p_prev  = p_curr;
        m = m_next;
        d = d_next;
        a_k = a_next;
        k++;
    }

    /* After the loop, p_prev holds p_n, the numerator of the nth convergent. */
    return p_prev;
}
