/*
 * A041528  Numerators of continued fraction convergents to sqrt(281).
 * https://oeis.org/A041528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the nth convergent of sqrt(281)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 18, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Continued‑fraction for sqrt(281)
       a0 = floor(sqrt(281)) = 16
       recurrence for m, d, a_k :
           m_{k+1} = d_k*a_k - m_k
           d_{k+1} = (D - m_{k+1}^2) / d_k
           a_{k+1} = (a0 + m_{k+1}) / d_{k+1}
       where D = 281.                                          */

    const uint64_t D = 281U;
    const uint64_t a0 = 16U;          /* floor(sqrt(281)) */

    /* Numerator recurrence: p_{-2}=0, p_{-1}=1 */
    seq_t p_im2 = 0;                  /* p_{-2} */
    seq_t p_im1 = 1;                  /* p_{-1} */
    seq_t p_i = 0;

    /* Denominator recurrence not needed for the answer,
       but provided for completeness: q_{-2}=1, q_{-1}=0 */
    seq_t q_im2 = 1;
    seq_t q_im1 = 0;
    seq_t q_i = 0;

    /* CF term variables */
    uint64_t m   = 0;          /* m_0 */
    uint64_t d   = 1;          /* d_0 */
    uint64_t a_k = a0;         /* a_0 */

    for (idx_t i = 0; i <= n; ++i)
    {
        /* compute the next numerator and denominator */
        p_i = (seq_t)a_k * p_im1 + p_im2;
        q_i = (seq_t)a_k * q_im1 + q_im2;

        /* shift for next iteration */
        p_im2 = p_im1;
        p_im1 = p_i;
        q_im2 = q_im1;
        q_im1 = q_i;

        /* stop after the required term */
        if (i == n) break;

        /* update m, d, a_k for the next CF term */
        uint64_t m_next = d * a_k - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a_k = a_next;
    }

    return p_im1;          /* numerator of the nth convergent */
}
