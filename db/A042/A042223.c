/*
 * A042223  Denominators of continued fraction convergents to sqrt(637).
 * https://oeis.org/A042223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth convergent of sqrt(637)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 19, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   Let m_0=0, d_0=1, a_0=⌊√637⌋=25.
   For k≥0:
      m_{k+1} = d_k · a_k − m_k
      d_{k+1} = (637 − m_{k+1}²) / d_k
      a_{k+1} = ⌊(25 + m_{k+1}) / d_{k+1}⌋
   Convergents:
      p_{-2}=0,  p_{-1}=1
      q_{-2}=1,  q_{-1}=0
      p_k = a_k · p_{k-1} + p_{k-2}
      q_k = a_k · q_{k-1} + q_{k-2}
   Then a(n) = q_n. */

#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t D  = 637;  /* the square‑free part */
    const uint64_t a0 = 25;   /* floor(sqrt(637)) */

    uint64_t m = 0;        /* m_k */
    uint64_t d = 1;        /* d_k */
    uint64_t a_k = a0;     /* a_k */

    seq_t p_prev2 = 0, p_prev1 = 1; /* p_{k-2}, p_{k-1} */
    seq_t q_prev2 = 1, q_prev1 = 0; /* q_{k-2}, q_{k-1} */

    for (idx_t k = 0; ; ++k)
    {
        /* current convergent */
        seq_t p = (seq_t)a_k * p_prev1 + p_prev2;
        seq_t q = (seq_t)a_k * q_prev1 + q_prev2;

        if (k == n)
            return q;

        /* compute next terms of the continued fraction */
        uint64_t m_next = d * a_k - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        /* shift for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p;
        q_prev2 = q_prev1;
        q_prev1 = q;

        m = m_next;
        d = d_next;
        a_k = a_next;
    }
}
