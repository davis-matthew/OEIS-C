/*
 * A008628  Molien series for A_5.
 * https://oeis.org/A008628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^4 + 10*n^3 + 160*n^2 + 675*n + 45*n*(-1)^n +
 *            1440)/1440)
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t N   = n;
    seq_t n2  = N * N;
    seq_t n3  = n2 * N;
    seq_t n4  = n2 * n2;

    seq_t num = n4
              + (seq_t)10 * n3
              + (seq_t)160 * n2
              + (seq_t)675 * N
              + (seq_t)1440;

    if ((n & 1) == 0) {          /* even n: (-1)^n = +1 */
        num += (seq_t)45 * N;
    } else {                     /* odd n: (-1)^n = -1 */
        num -= (seq_t)45 * N;
    }

    return num / (seq_t)1440;
}
