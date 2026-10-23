/*
 * A040677  Continued fraction for sqrt(704).
 * https://oeis.org/A040677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction terms for sqrt(704)
 * Range:     u8 none, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* Return the n-th term (0‑based) of the continued fraction expansion of sqrt(704). */
seq_t a(idx_t n)
{
    const seq_t D = (seq_t)704;
    seq_t a0 = floor_sqrt(D);          /* a(0) */
    if (n == 0) return a0;

    seq_t m = 0;                       /* m_0 */
    seq_t d = 1;                       /* d_0 */
    seq_t an = a0;                     /* a_k, start with a_0 */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t m_next = d * an - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        an = a_next;
    }
    return an;
}
