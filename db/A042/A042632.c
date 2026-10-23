/*
 * A042632  Numerators of continued fraction convergents to sqrt(846).
 * https://oeis.org/A042632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 11, u32 n <= 19, u64 n <= 46, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numerators of continued fraction convergents to sqrt(846). */
/* Recurrence:
   a_0 = floor(sqrt(846)) = 29
   For k ≥ 1:
        m_k   = d_{k-1} * a_{k-1} - m_{k-1}
        d_k   = (846 - m_k^2) / d_{k-1}
        a_k   = floor((a_0 + m_k) / d_k)
   Numerator recurrence:
        p_{-2} = 0,  p_{-1} = 1
        p_k   = a_k * p_{k-1} + p_{k-2}   (k ≥ 0)
   a(0) = p_0 = A_0 = 29
   a(n) = p_n
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Radical */
    const idx_t D  = 846;
    const idx_t A0 = 29;          /* floor(sqrt(D)) */

    /* Handle the first term */
    if (n == 0) {
        return (seq_t)A0;         /* p_0 = A0 */
    }

    /* State for continued fraction expansion */
    idx_t m = 0;                  /* m_0   */
    idx_t d = 1;                  /* d_0   */
    idx_t a_cf = A0;              /* a_0   */

    /* States for numerator recurrence */
    seq_t p_prev2 = (seq_t)0;     /* p_{-2} */
    seq_t p_prev1 = (seq_t)1;     /* p_{-1} */
    seq_t p_curr  = (seq_t)A0;    /* p_0    */

    /* After computing p_0 we shift the numerator states */
    p_prev2 = p_prev1;            /* now p_prev2 = p_{-1} = 1 */
    p_prev1 = p_curr;             /* now p_prev1 = p_0     = 29 */

    /* Iterate from k = 1 up to n */
    for (idx_t k = 1; k <= n; ++k) {
        /* Compute next continued fraction coefficient a_k */
        idx_t m_next = d * a_cf - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (A0 + m_next) / d_next;

        /* Update the continued fraction state */
        m = m_next;
        d = d_next;
        a_cf = a_next;

        /* Numerator recurrence: p_k = a_k * p_{k-1} + p_{k-2} */
        p_curr = (seq_t)a_next * p_prev1 + p_prev2;

        /* Shift for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }

    return p_curr;
}
