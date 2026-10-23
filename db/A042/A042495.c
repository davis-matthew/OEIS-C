/*
 * A042495  Denominators of continued fraction convergents to sqrt(775).
 * https://oeis.org/A042495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) is the denominator of the n-th convergent to sqrt(775)
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    const seq_t D = (seq_t)775ULL;      /* radicand */
    /* a0 = floor(sqrt(D)) */
    seq_t a0 = floor_sqrt(D);

    /* Initial state for continued‑fraction calculation */
    seq_t m = 0;          /* m_{k-1} */
    seq_t d = 1;          /* d_{k-1} */
    seq_t a_cur = a0;     /* a_k */

    /* Recurrence for convergent denominators q_k */
    seq_t q_prev2 = 1;    /* q_{-2} */
    seq_t q_prev1 = 0;    /* q_{-1} */
    seq_t q = 0;          /* q_k */

    /* Compute coefficients and denominators up to index n */
    for (idx_t i = 0; i <= n; ++i)
    {
        if (i > 0)      /* compute next coefficient a_k for k>=1 */
        {
            seq_t m_next = d * a_cur - m;                /* m_k   */
            seq_t d_next = (D - m_next * m_next) / d;    /* d_k   */
            a_cur = (a0 + m_next) / d_next;              /* a_k   */
            m = m_next;
            d = d_next;
        }

        q = a_cur * q_prev1 + q_prev2;   /* q_k = a_k*q_{k-1} + q_{k-2} */
        q_prev2 = q_prev1;
        q_prev1 = q;
    }

    return q_prev1;    /* q_n */
}
