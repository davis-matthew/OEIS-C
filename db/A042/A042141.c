/*
 * A042141  Denominators of continued fraction convergents to sqrt(595).
 * https://oeis.org/A042141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 22, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of denominators of continued‑fraction convergents to sqrt(595).
   The denominators q_n satisfy:
     q_{-2} = 1,  q_{-1} = 0
     q_n   = a_n * q_{n-1} + q_{n-2}
   where a_0 = 24 and the subsequent partial quotients a_{k+1} are
   obtained from the standard continued‑fraction algorithm for sqrt(595).
   Thus a(0)=1, a(1)=2, a(2)=3, a(3)=5, ...   */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Constants for sqrt(595) */
    const int64_t a0  = 24;  /* floor(sqrt(595)) */
    const int64_t D   = 595; /* the radicand */
    int64_t m = 0;
    int64_t d = 1;
    int64_t ai = a0;   /* current partial quotient */

    /* Initialize previous two denominators: q_{-2}=1, q_{-1}=0 */
    seq_t q_prev2 = (seq_t)1;
    seq_t q_prev1 = (seq_t)0;

    /* Compute up to the requested index n */
    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t q_i = (seq_t)ai * q_prev1 + q_prev2;
        if (i == n)
            return q_i;

        /* Shift denominators for next iteration */
        q_prev2 = q_prev1;
        q_prev1 = q_i;

        /* Next partial quotient via the continued‑fraction recurrence */
        int64_t m_next = d * ai - m;
        int64_t d_next = (D - m_next * m_next) / d;
        int64_t ai_next = (a0 + m_next) / d_next;

        m  = m_next;
        d  = d_next;
        ai = ai_next;
    }

    /* Unreachable, but required to satisfy all code paths */
    return (seq_t)0;
}
