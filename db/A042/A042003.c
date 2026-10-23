/*
 * A042003  Denominators of continued fraction convergents to sqrt(524).
 * https://oeis.org/A042003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 13, u32 n <= 22, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued fraction convergents to sqrt(524). */
#include "../utils/sqrt_floor.h"

/* Computes a(n) = denominator of the (n‑th) convergent of sqrt(524).
   a(0) = 1, a(1) = 1, a(2) = 9, ...  (see OEIS A042003). */
seq_t a(idx_t n)
{
    const seq_t S = (seq_t)524;
    const seq_t a0 = sqrt_floor(S);          /* floor(sqrt(524)) = 22 */

    if (n == 0) return (seq_t)1;             /* q0 = 1 */

    seq_t m = (seq_t)0;
    seq_t d = (seq_t)1;
    seq_t a_cur = a0;                        /* current a_k */

    /* Denominator recurrence: q_{-1}=0, q0=1. */
    seq_t q_prev2 = (seq_t)0;                /* q_{k-2}   */
    seq_t q_prev1 = (seq_t)1;                /* q_{k-1}   */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Next terms of the continued fraction of sqrt(S). */
        seq_t m_next = d * a_cur - m;
        seq_t d_next = (S - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        /* Update denominators: q_k = a_k * q_{k-1} + q_{k-2}. */
        seq_t q_curr = a_next * q_prev1 + q_prev2;

        /* Shift for next iteration. */
        q_prev2 = q_prev1;
        q_prev1 = q_curr;
        m = m_next;
        d = d_next;
        a_cur = a_next;
    }

    return q_prev1;
}
