/*
 * A040760  Continued fraction for sqrt(789).
 * https://oeis.org/A040760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) is the n-th partial quotient of the periodic continued
 *            fraction expansion of sqrt(789)
 * Range:     u8 none, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    const seq_t D   = (seq_t)789;
    const seq_t a0  = floor_sqrt(D);
    if (n == 0) return a0;

    seq_t m      = (seq_t)0;
    seq_t d      = (seq_t)1;
    seq_t a_cur  = a0;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t m_next = d * a_cur - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;
        m = m_next;
        d = d_next;
        a_cur = a_next;
    }
    return a_cur;
}
