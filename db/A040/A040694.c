/*
 * A040694  Continued fraction for sqrt(721).
 * https://oeis.org/A040694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = n-th partial quotient of continued fraction for sqrt(721)
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const seq_t a0 = 26;               /* floor(sqrt(721)) */
    if (n == 0) return a0;

    idx_t m = 0;
    idx_t d = 1;
    seq_t a = a0;

    for (idx_t i = 1; i <= n; ++i) {
        /* compute the next m */
        m = d * (idx_t)a - m;

        /* compute the next d */
        idx_t mm = m * m;                /* m*m fits in idx_t (m < sqrt(721)) */
        d = (721 - mm) / d;              /* exact division */

        /* compute the next a (partial quotient) */
        a = (seq_t)((a0 + m) / d);       /* integer division does floor */
    }

    return a;
}
