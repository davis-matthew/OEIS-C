/*
 * A042649  Denominators of continued fraction convergents to sqrt(854).
 * https://oeis.org/A042649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued‑fraction denominators of √854.
   Uses the standard recurrence for convergents:
   p_k = a_k p_{k-1} + p_{k-2},
   q_k = a_k q_{k-1} + q_{k-2}                           */
/* a(0)=1, a(1)=4, a(2)=9, a(3)=103, ... */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;  /* q_0 = 1 */

    const uint64_t D = 854ULL;     /* the square‑free part */
    const uint64_t a0 = 29ULL;     /* floor(sqrt(854)) */

    seq_t q_prev2 = 0;  /* q_{k-2} */
    seq_t q_prev1 = 1;  /* q_{k-1} initially q_0 */

    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a_cur = a0;   /* a_k */

    for (idx_t i = 1; i <= n; ++i) {
        /* next partial quotient */
        uint64_t m_next = d * a_cur - m;

        /* (D - m_next^2) fits in 128‑bit */
        unsigned __int128 diff = (unsigned __int128)D
                               - (unsigned __int128)m_next * m_next;
        uint64_t d_next = (uint64_t)(diff / d);   /* guaranteed integer */
        uint64_t a_next = (a0 + m_next) / d_next;

        /* compute next denominator q_k */
        seq_t q_next = (seq_t)a_next * q_prev1 + q_prev2;

        /* shift state for next iteration */
        q_prev2 = q_prev1;
        q_prev1 = q_next;
        m = m_next;
        d = d_next;
        a_cur = a_next;
    }

    return q_prev1;  /* q_n */
}
