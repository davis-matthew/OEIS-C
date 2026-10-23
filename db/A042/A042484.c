/*
 * A042484  Numerators of continued fraction convergents to sqrt(770).
 * https://oeis.org/A042484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 222*a(n-4) - a(n-8); a(0)=27, a(1)=28, a(2)=83, a(3)=111,
 *            a(4)=6077, a(5)=6188, a(6)=18453, a(7)=24641
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t init[8] = {
        (seq_t)27, (seq_t)28, (seq_t)83, (seq_t)111,
        (seq_t)6077, (seq_t)6188, (seq_t)18453, (seq_t)24641
    };
    if (n < 8) {
        return init[n];
    }
    seq_t buf[8];
    for (int i = 0; i < 8; ++i) {
        buf[i] = init[i];
    }
    for (idx_t i = 8; i <= n; ++i) {
        idx_t pos  = i & 7;
        idx_t pos4 = (i - 4) & 7;
        idx_t pos8 = (i - 8) & 7;
        buf[pos] = 222 * buf[pos4] - buf[pos8];
    }
    return buf[n & 7];
}
