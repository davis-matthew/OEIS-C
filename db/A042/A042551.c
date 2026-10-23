/*
 * A042551  Denominators of continued fraction convergents to sqrt(804).
 * https://oeis.org/A042551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth convergent of √804 in its *
 *            continued‐fraction expansion. The recurrence uses the standard *
 *            continued–fraction algorithm: * * m_{k+1} = d_k * a_k - m_k *
 *            d_{k+1} = (D - m_{k+1}²) / d_k * a_{k+1} = ⎣ (a₀ + m_{k+1}) /
 *            d_{k+1} ⎦ * * Convergents: * p_{-2}=0, p_{-1}=1 *...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 16, u64 n <= 33, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t D     = 804ULL;   /* the radicand */
    const uint64_t a0    = 28ULL;    /* floor(sqrt(804)) */

    /* Continued‑fraction state */
    uint64_t m   = 0;
    uint64_t d   = 1;
    uint64_t a_k = a0;              /* current a_k, starts with a0 at k = 0 */

    /* Convergent state */
    seq_t q_prev2 = 1;              /* q_{-2} */
    seq_t q_prev1 = 0;              /* q_{-1} */
    seq_t q;                         /* q_k */

    for (idx_t k = 0; k <= n; ++k) {
        q = (seq_t)a_k * q_prev1 + q_prev2;
        if (k == n)
            return q;

        /* Compute next term of continued fraction */
        uint64_t m_next = d * a_k - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        /* Advance states for next iteration */
        m = m_next;
        d = d_next;
        a_k = a_next;

        q_prev2 = q_prev1;
        q_prev1 = q;
    }

    /* Should never reach here */
    return 0;
}
