/*
 * A042386  Numerators of continued fraction convergents to sqrt(720).
 * https://oeis.org/A042386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = Numerator of the nth convergent of sqrt(720) continued
 *            fraction. The partial quotients are a0 = 26, then repeat the
 *            period [1, 4, 1, 52]. Recurrence: h[-2] = 0, h[-1] = 1; h[i] =
 *            a_i * h[i-1] + h[i-2]; a( n ) = h[n]
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* period of partial quotients after the first term */
    static const seq_t period[4] = { (seq_t)1, (seq_t)4, (seq_t)1, (seq_t)52 };

    seq_t h_minus1 = (seq_t)1;   /* h[-1] */
    seq_t h_minus2 = (seq_t)0;   /* h[-2] */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t a_i = (i == 0) ? (seq_t)26 : period[(i - 1) & 3];
        seq_t cur  = a_i * h_minus1 + h_minus2;
        h_minus2  = h_minus1;
        h_minus1  = cur;
    }
    return h_minus1;
}
