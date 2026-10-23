/*
 * A042243  Denominators of continued fraction convergents to sqrt(647).
 * https://oeis.org/A042243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = denominator of the n-th convergent of the continued fraction for sqrt(647) 
   (this produces the sequence A042243). 
*/

#include <stdint.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /*--------------------------------------------------------------------
      √647 = 25 + …,  a0 = 25
      For a non‑square integer D we use the standard recurrence:
            m₀ = 0, d₀ = 1, a₀ = ⌊√D⌋
            m_{k+1} = d_k·a_k − m_k
            d_{k+1} = (D − m_{k+1}²)/d_k
            a_{k+1} = ⌊(a₀ + m_{k+1})/d_{k+1}⌋
      Convergents (p_k , q_k) are obtained from
            p_{−2}=0, p_{−1}=1,   q_{−2}=1, q_{−1}=0
            p_k = a_k·p_{k−1} + p_{k−2}
            q_k = a_k·q_{k−1} + q_{k−2}
      a(n) = q_n .
      --------------------------------------------------------------------*/

    /* constants for the quadratic √647 */
    const seq_t D   = (seq_t)647;               /* D = 647   */
    const seq_t a0s = floor_sqrt(D);           /* a0 = ⌊√647⌋ = 25   */
    const idx_t  a0  = (idx_t)a0s;             /* a0 as plain integer */

    /* continued fraction variables (seq_t for safety) */
    seq_t m   = (seq_t)0;        /* m₀   */
    seq_t d   = (seq_t)1;        /* d₀   */
    seq_t a_k = a0s;             /* a₀   */

    /* convergent variables */
    seq_t p_prev2 = (seq_t)0;    /* p_{−2} */
    seq_t p_prev1 = (seq_t)1;    /* p_{−1} */
    seq_t q_prev2 = (seq_t)1;    /* q_{−2} */
    seq_t q_prev1 = (seq_t)0;    /* q_{−1} */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* compute q_k and p_k */
        seq_t p = a_k * p_prev1 + p_prev2;
        seq_t q = a_k * q_prev1 + q_prev2;

        if (k == n)
            return q;                 /* returned as seq_t */

        /* shift for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p;
        q_prev2 = q_prev1;
        q_prev1 = q;

        /* next partial quotient a_{k+1} */
        seq_t m_next = d * a_k - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (a0s + m_next) / d_next;   /* floor division   */

        /* update state */
        m = m_next;
        d = d_next;
        a_k = a_next;
    }

    /* this point is never reached, but keep the compiler happy */
    return (seq_t)0;
}
