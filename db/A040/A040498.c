/*
 * A040498  Continued fraction for sqrt(521).
 * https://oeis.org/A040498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) is the n‑th partial quotient in the simple continued *
 *            fraction expansion of √521. * Implementation uses the standard
 *            recurrence: * * a0 = ⌊√521⌋ * m0 = 0, d0 = 1 * for k ≥ 1: * m_k
 *            = d_{k‑1} * a_{k‑1} – m_{k‑1} * d_k = (521 – m_k²) / d_{k‑1} *
 *            a_k = ⌊(a0 + m_k) / d_k⌋ * * The function recom...
 * Range:     u8 none, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Integer part of √521 */
    const seq_t a0 = floor_sqrt((seq_t)521);

    /* a(0) is simply a0 */
    if (n == 0)
        return a0;

    seq_t m = 0;      /* m_{k-1} */
    seq_t d = 1;      /* d_{k-1} */
    seq_t aik = a0;   /* a_{k-1} */

    /* Iterate up to the desired index */
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t m_next = d * aik - m;
        seq_t d_next = ((seq_t)521 - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        if (k == n)
            return a_next;

        /* Prepare for next iteration */
        m = m_next;
        d = d_next;
        aik = a_next;
    }

    /* Unreachable, but keep compiler happy */
    return 0;
}
