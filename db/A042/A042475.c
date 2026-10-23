/*
 * A042475  Denominators of continued fraction convergents to sqrt(765).
 * https://oeis.org/A042475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = denominator of the nth continued‑fraction convergent of √765.
   Recurrence:
       let m0 = 0, d0 = 1, a0 = ⌊√765⌋ = 27.
       for k ≥ 0:
           m_{k+1} = d_k * a_k – m_k
           d_{k+1} = (765 – m_{k+1}²) / d_k
           a_{k+1} = ⌊(27 + m_{k+1}) / d_{k+1}⌋
       convergents (p_k, q_k) with
           q_{-2} = 1, q_{-1} = 0
           q_k   = a_k * q_{k-1} + q_{k-2}
   a(0) = 1, a(1) = 1, a(2) = 2, … as per OEIS A042475. */

#include <stdint.h>

seq_t a(idx_t n)
{
    const unsigned D   = 765;             /* the square‑free part */
    const unsigned a0  = 27;              /* floor(sqrt(765)) */

    unsigned long long m = 0;               /* m_k */
    unsigned long long d = 1;               /* d_k */
    unsigned long long a = a0;              /* a_k */

    /* base values of the denominator recurrence: q_{-2}=1, q_{-1}=0 */
    seq_t q_prev2 = 1;                      /* q_{k-2} */
    seq_t q_prev1 = 0;                      /* q_{k-1} */
    seq_t q_k;                              /* current q_k */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* q_k = a_k * q_{k-1} + q_{k-2} */
        q_k = (seq_t)a * q_prev1 + q_prev2;

        if (k == n)
            return q_k;

        /* shift for next step */
        q_prev2 = q_prev1;
        q_prev1 = q_k;

        /* compute the next partial quotient a_{k+1} */
        unsigned long long m_next = d * a - m;
        unsigned long long d_next = (D - m_next * m_next) / d;
        unsigned long long a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a = a_next;
    }

    /* unreachable: the function must return before this point */
    return 0;
}
