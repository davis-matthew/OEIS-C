/*
 * A040764  Continued fraction for sqrt(793).
 * https://oeis.org/A040764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = terms of the continued fraction expansion of sqrt(793)
 *            (a0 = floor(sqrt(793)) = 28, recurrence: m_{k+1} = d_k * a_k -
 *            m_k d_{k+1} = (793 - m_{k+1}^2) / d_k a_{k+1} = floor((28 +
 *            m_{k+1}) / d_{k+1}) )
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)28U;

    const uint64_t D  = 793ULL;  /* D = 793 */
    const uint64_t a0 = 28ULL;   /* a0 = floor(sqrt(793)) */
    uint64_t m = 0, d = 1, a_val = a0;
    for (idx_t i = 1; i <= n; ++i) {
        uint64_t m_next = d * a_val - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;
        m = m_next;
        d = d_next;
        a_val = a_next;
    }
    return (seq_t)a_val;
}
