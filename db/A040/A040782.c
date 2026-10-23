/*
 * A040782  Continued fraction for sqrt(811).
 * https://oeis.org/A040782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the n-th term of the continued fraction expansion of sqrt(811). 
   The algorithm uses the standard recurrence for quadratic surds. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    const seq_t D = (seq_t)811;           /* the radicand */
    seq_t a0 = floor_sqrt(D);             /* a(0) = floor(sqrt(811)) = 28 */

    if (n == 0) return a0;

    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;                         /* a(0) is already known */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t m_next = d * a - m;                     /* m_{k+1} = d_k * a_k - m_k */
        seq_t d_next = D - m_next * m_next;           /* d_{k+1} = (D - m_{k+1}^2) / d_k */
        d_next = d_next / d;
        seq_t a_next = (a0 + m_next) / d_next;        /* a_{k+1} = floor((a0 + m_{k+1}) / d_{k+1}) */

        m = m_next;
        d = d_next;
        a = a_next;
    }
    return a;
}
