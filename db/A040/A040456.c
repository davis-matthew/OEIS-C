/*
 * A040456  Continued fraction for sqrt(478).
 * https://oeis.org/A040456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(478).
   a(0) = 21.
   Recurrence:
     m_{k+1} = a_k * d_k – m_k
     d_{k+1} = (N – m_{k+1}²) / d_k
     a_{k+1} = ⌊ (a0 + m_{k+1}) / d_{k+1} ⌋
   where N = 478 and a0 = 21. */
#include <stdint.h>

seq_t a(idx_t n)
{
    const idx_t N  = 478;
    const idx_t a0 = 21;

    if (n == 0)
        return (seq_t)a0;

    idx_t m = 0;
    idx_t d = 1;
    idx_t ak = a0;

    for (idx_t i = 1; i <= n; ++i) {
        idx_t m_next = ak * d - m;
        idx_t d_next = (N - m_next * m_next) / d;
        ak = (a0 + m_next) / d_next;
        m  = m_next;
        d  = d_next;
    }

    return (seq_t)ak;
}
