/*
 * A156934  G.f. of the z^1 coefficients of the FP2 in the second column of
 *          the A156925 matrix.
 * https://oeis.org/A156934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1)-12*a(n-2)+10*a(n-3)-3*a(n-4)+2 with initial
 *            values a(1)=1, a(2)=8, a(3)=38, a(4)=144
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n <= 4) {
        static const seq_t base[4] = {1, 8, 38, 144};
        return base[n-1];
    }
    seq_t a1 = 1;
    seq_t a2 = 8;
    seq_t a3 = 38;
    seq_t a4 = 144;
    seq_t an;
    for (idx_t i = 5; i <= n; ++i) {
        an = 6*a4 - 12*a3 + 10*a2 - 3*a1 + 2;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return a4;
}
