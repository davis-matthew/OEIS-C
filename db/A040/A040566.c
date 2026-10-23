/*
 * A040566  Continued fraction for sqrt(591).
 * https://oeis.org/A040566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Continued fraction partial quotients for sqrt(591)
   Recurrence:
       m_{k+1} = d_k * a_k - m_k
       d_{k+1} = (591 - m_{k+1}^2) / d_k
       a_{k+1} = (24 + m_{k+1}) / d_{k+1}
   Initial values:
       m_0 = 0, d_0 = 1, a_0 = 24
   a(0) = 24, a(1) = 3, a(2) = 4, a(3) = 1, ...  (as in A040566)
*/

seq_t a(idx_t n) {
    const seq_t N = (seq_t)591;     /* constant N = 591 */
    const seq_t a0 = (seq_t)24;     /* floor(sqrt(591)) */
    if (n == 0) return a0;

    seq_t m = (seq_t)0;
    seq_t d = (seq_t)1;
    seq_t a_val = a0;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t m_next = d * a_val - m;
        seq_t d_next = (N - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;
        m = m_next;
        d = d_next;
        a_val = a_next;
    }
    return a_val;
}
