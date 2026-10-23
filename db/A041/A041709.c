/*
 * A041709  Denominators of continued fraction convergents to sqrt(374).
 * https://oeis.org/A041709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued fraction convergents to sqrt(374).  See A041709. */
/* Recurrence: let a0 = 19 and generate the continued‑fraction coefficients a_k
   by the standard algorithm with m0 = 0, d0 = 1.  Set q_{-2} = 1, q_{-1} = 0.
   For k ≥ 0:      q_k = a_k * q_{k-1} + q_{k-2}.  
   The function returns q_n. */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1u;

    seq_t q_prev2 = (seq_t)1u;   /* q_{-2} */
    seq_t q_prev1 = (seq_t)0u;   /* q_{-1} */
    seq_t q = 0;

    idx_t m = 0u;                 /* m_0 */
    idx_t d = 1u;                 /* d_0 */
    const idx_t a0 = 19u;         /* floor(sqrt(374)) */
    idx_t a = a0;                 /* a_0 */

    for (idx_t i = 0u; i <= n; ++i)
    {
        /* compute q_i = a * q_{i-1} + q_{i-2} */
        q = (seq_t)a * q_prev1 + q_prev2;

        /* shift past terms for next iteration */
        q_prev2 = q_prev1;
        q_prev1 = q;

        if (i == n)
            break;

        /* compute data for next coefficient */
        idx_t m_next = d * a - m;                     /* m_{i+1} */
        idx_t d_next = (374u - (idx_t)(m_next * m_next)) / d; /* d_{i+1} */
        idx_t a_next = (a0 + m_next) / d_next;        /* a_{i+1} */

        /* update state */
        m = m_next;
        d = d_next;
        a = a_next;
    }

    return q;
}
