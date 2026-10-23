/*
 * A040666  Continued fraction for sqrt(693).
 * https://oeis.org/A040666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction terms for sqrt(693).
   Implements the classical sqrt continued fraction algorithm:
   m_{k+1} = d_k * a_k - m_k
   d_{k+1} = (D - m_{k+1}^2) / d_k
   a_{k+1} = floor((a0 + m_{k+1}) / d_{k+1})
   with D = 693 and a0 = floor(sqrt(693)) = 26. */
#include <stdint.h>

seq_t a(idx_t n) {
    const seq_t D  = 693;
    const seq_t a0 = 26; /* floor(sqrt(693)) */

    if (n == 0) {
        return a0;
    }

    seq_t m = 0;
    seq_t d = 1;
    seq_t a_val = a0;

    for (idx_t i = 1; i <= n; ++i) {
        /* compute next m, d, a */
        m = d * a_val - m;
        d = (D - m * m) / d;
        a_val = (a0 + m) / d;
    }
    return a_val;
}
