/*
 * A010210  Continued fraction for sqrt(161).
 * https://oeis.org/A010210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   Continued fraction coefficient of sqrt(161), a(0) = 12. Computed
 *            by the standard recurrence for quadratic surds
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const seq_t D   = (seq_t)161;   /* D = 161 */
    const seq_t a0  = (seq_t)12;    /* floor(sqrt(161)) */

    if (n == 0) return a0;

    seq_t m = (seq_t)0;
    seq_t d = (seq_t)1;
    seq_t a = a0;

    /* Iterate until the nth term is reached. */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t m_next = d * a - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a = a_next;
    }
    return a;
}
