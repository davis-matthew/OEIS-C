/*
 * A041347  Denominators of continued fraction convergents to sqrt(187).
 * https://oeis.org/A041347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A041347: Denominators of continued fraction convergents to sqrt(187)
 * The function computes the n-th denominator q_n of the convergents to √187
 * using the standard recurrence for continued fractions:
 *   m_{k+1} = d_k * a_k - m_k
 *   d_{k+1} = (187 - m_{k+1}^2) / d_k
 *   a_{k+1} = ⌊(13 + m_{k+1}) / d_{k+1}⌋
 *   q_{k+1} = a_{k+1} * q_k + q_{k-1}
 * with initial values q_{-1} = 0, q_0 = 1, m_0 = 0, d_0 = 1, a_0 = 13.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case n == 0 */
    if (n == 0)
        return (seq_t)1;   /* q_0 = 1 */

    /* Variables for continued fraction */
    idx_t m    = 0;      /* m_0 */
    idx_t d    = 1;      /* d_0 */
    idx_t a_k  = 13;     /* a_0 */

    /* Denominator recurrence variables */
    seq_t q_prev = (seq_t)0; /* q_{-1} */
    seq_t q_curr = (seq_t)1; /* q_0 */

    /* Temporary variables for the next step */
    seq_t q_next;

    /* Iterate from k = 1 to n, computing q_k each time */
    for (idx_t k = 1; k <= n; ++k) {
        /* Compute next continued fraction coefficients */
        idx_t m_next  = d * a_k - m;
        idx_t d_next  = (187 - m_next * m_next) / d;
        idx_t a_next  = (13 + m_next) / d_next;   /* floor division */

        /* Compute next denominator */
        q_next = (seq_t)a_next * q_curr + q_prev;

        /* Shift variables for next iteration */
        q_prev = q_curr;
        q_curr = q_next;
        m      = m_next;
        d      = d_next;
        a_k    = a_next;
    }

    return q_curr;
}
