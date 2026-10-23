/*
 * A042670  Numerators of continued fraction convergents to sqrt(865).
 * https://oeis.org/A042670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent for sqrt(865)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continuing fraction for sqrt(865) with recurrence:
 *   P_{-1} = 1, P_0 = 29
 *   P_k   = a_k*P_{k-1} + P_{k-2}
 *   a_0   = 29
 *   For k≥1:
 *      m_k   = d_{k-1}*a_{k-1} – m_{k-1}
 *      d_k   = (865 – m_k*m_k) / d_{k-1}
 *      a_k   = (29 + m_k) / d_k            (floor division)
 */
 
#include <stdint.h>

seq_t a(idx_t n)
{
    const seq_t a0 = (seq_t)29;     /* floor(sqrt(865)) */
    if (n == 0) return a0;

    /* recurrence variables */
    seq_t p_prev2 = (seq_t)1;      /* P_{-1} */
    seq_t p_prev1 = a0;            /* P_0 */

    /* continued–fraction parameters (values fit in 64‑bit) */
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a_cur = 29;           /* a_0 */

    for (idx_t k = 1; k <= n; ++k) {
        /* compute next a_k following standard algorithm */
        uint64_t m_next = d * a_cur - m;
        uint64_t d_next = (865ULL - m_next * m_next) / d;
        uint64_t a_next = (29 + m_next) / d_next;   /* floor division */

        /* compute numerator P_k */
        seq_t p_k = (seq_t)a_next * p_prev1 + p_prev2;

        /* shift for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p_k;
        m = m_next;
        d = d_next;
        a_cur = a_next;
    }

    return p_prev1;
}
