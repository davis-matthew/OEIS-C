/*
 * A042229  Denominators of continued fraction convergents to sqrt(640).
 * https://oeis.org/A042229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 19, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued‑fraction convergents to sqrt(640).
 * The partial quotients a_k of sqrt(640) are produced by the standard
 * continued‑fraction algorithm:
 *   m0 = 0, d0 = 1, a0 = floor(sqrt(640)) = 25
 *   For k >= 0:
 *     m_{k+1} = d_k * a_k - m_k
 *     d_{k+1} = (640 - m_{k+1}^2) / d_k
 *     a_{k+1} = floor((a0 + m_{k+1}) / d_{k+1})
 * The denominators of the convergents q_k satisfy
 *   q_{-2} = 1, q_{-1} = 0
 *   q_k   = a_k * q_{k-1} + q_{k-2}   (k >= 0)
 * This function returns q_n for the requested index n.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Constants for the continued fraction of sqrt(640) */
    const uint64_t D   = 640;   /* 640 = 8^2 * 10 */
    const int      a0  = 25;    /* floor(sqrt(640)) */

    /* Continued fraction state variables */
    int64_t m  = 0;      /* m_0 */
    int64_t d  = 1;      /* d_0 */
    int    ak = a0;     /* a_k, starting with a_0 */

    /* Denominator recurrence state */
    seq_t q_prev2 = 1;   /* q_{k-2}  */
    seq_t q_prev1 = 0;   /* q_{k-1}  */

    for (idx_t k = 0; k <= n; ++k) {
        /* Compute the denominator for this k */
        seq_t qk = (seq_t)ak * q_prev1 + q_prev2;

        /* Return if this is the requested term */
        if (k == n) {
            return qk;
        }

        /* Advance to the next partial quotient (k+1) */
        int64_t m_next  = d * ak - m;
        int64_t d_next  = (D - m_next * m_next) / d;
        int    ak_next  = (a0 + m_next) / d_next;

        m  = m_next;
        d  = d_next;
        ak = ak_next;

        /* Shift denominator recurrence variables */
        q_prev2 = q_prev1;
        q_prev1 = qk;
    }

    /* Unreachable, but satisfies the compiler */
    return 0;
}
