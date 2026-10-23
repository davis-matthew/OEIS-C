/*
 * A042021  Denominators of continued fraction convergents to sqrt(534).
 * https://oeis.org/A042021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Denominators of continued fraction convergents to sqrt(534)
   Recurrence (farey type):
   Let a0 = floor(sqrt(534)) = 23.
   m_0 = 0, d_0 = 1, a_0 = 23.
   For k ≥ 0:
       m_{k+1} = d_k * a_k – m_k
       d_{k+1} = (534 – m_{k+1}^2) / d_k
       a_{k+1} = floor((a0 + m_{k+1}) / d_{k+1})
   Denominator recurrence:
       q_{-2} = 1, q_{-1} = 0
       q_k   = a_k * q_{k-1} + q_{k-2}
   Then a(n) = q_n  (a(0) = 1).  */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case */
    if (n == (idx_t)0) return (seq_t)1;

    /* Initial parameters for sqrt(534) */
    const uint64_t a0 = 23;           /* floor(sqrt(534)) */
    uint64_t m = 0, d = 1;            /* m_0, d_0 */
    uint64_t a_k = a0;                /* a_0 */

    /* Denominator recurrence */
    seq_t q;                          /* current denominator q_k */
    seq_t prev2 = (seq_t)1;           /* q_{-2} */
    seq_t prev1 = (seq_t)0;           /* q_{-1} */

    /* k = 0: q_0 = a0 * prev1 + prev2 = 1 */
    q = (seq_t)1;                     /* q_0 */
    prev2 = prev1;                    /* prev2 = q_{-1} == 0 */
    prev1 = q;                        /* prev1 = q_0 == 1 */

    /* Iterate for k = 1 .. n */
    for (idx_t k = (idx_t)1; k <= n; ++k)
    {
        /* Extended continued-fraction step */
        uint64_t m_next = d * a_k - m;
        uint64_t d_next = (534 - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        /* Denominator recurrence */
        q = (seq_t)a_next * prev1 + prev2;

        /* Prepare for next iteration */
        prev2 = prev1;
        prev1 = q;

        m = m_next;
        d = d_next;
        a_k = a_next;
    }

    return q;
}
