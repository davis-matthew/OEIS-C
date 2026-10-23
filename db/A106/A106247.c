/*
 * A106247  Expansion of (1+2*x-x^2-2*x^3+x^4) / (1-x^2)^3.
 * https://oeis.org/A106247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^2+2*n+8)/8 for even n a(n) = n+1 for odd n
 * Range:     u8 n <= 45, u16 n <= 723, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {               /* even */
        seq_t nn = (seq_t)n;
        /* numerator is guaranteed to be a multiple of 8 */
        return (nn*nn + 2*nn + 8) / 8;
    } else {                           /* odd */
        return (seq_t)n + 1;
    }
}
