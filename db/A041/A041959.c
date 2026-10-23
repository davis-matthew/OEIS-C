/*
 * A041959  Denominators of continued fraction convergents to sqrt(502).
 * https://oeis.org/A041959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of the denominators of the convergents to
   sqrt(502).  The coefficients of the simple continued fraction of
   sqrt(502) are generated on the fly, and the standard recurrence
   for convergents is used:
      q_{-1} = 0, q_0 = 1
      p_{-1} = 1, p_0 = a0
      for k ≥ 1:
          q_k = a_k * q_{k-1} + q_{k-2}
          p_k = a_k * p_{k-1} + p_{k-2}   (not needed here)
   where a0 = ⌊√502⌋ = 22 and the subsequent a_k are obtained by
   the classical Pell‑type recurrence for quadratic surds. */

#include <stdint.h>

seq_t a(idx_t n)
{
    const seq_t D  = (seq_t)502;   /* non‑square integer */
    const seq_t a0 = (seq_t)22;    /* floor(sqrt(502)) */

    /* Continued fraction iteration variables */
    seq_t m  = 0;
    seq_t d  = 1;
    seq_t a_coeff = a0;

    /* Convergents p, q */
    seq_t p_prev2 = 1;   /* p_-1 */
    seq_t p_prev  = a0;  /* p_0 */

    seq_t q_prev2 = 0;   /* q_-1 */
    seq_t q_prev  = 1;   /* q_0 */

    if (n == 0)
        return q_prev;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* next CF coefficient */
        seq_t m_next  = d * a_coeff - m;
        seq_t d_next  = (D - m_next * m_next) / d;
        seq_t a_next  = (a0 + m_next) / d_next;

        /* recurrence for convergents */
        seq_t p_cur = a_next * p_prev + p_prev2;
        seq_t q_cur = a_next * q_prev + q_prev2;

        /* shift for next iteration */
        p_prev2 = p_prev;  p_prev = p_cur;
        q_prev2 = q_prev;  q_prev = q_cur;

        /* update CF variables */
        m = m_next;  d = d_next;  a_coeff = a_next;
    }

    return q_prev;
}
