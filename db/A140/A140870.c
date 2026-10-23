/*
 * A140870  8*P_4(2n), 8 times the Legendre Polynomial of order 4 at 2n.
 * https://oeis.org/A140870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 560*n^4 - 120*n^2 + 3
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n4 = n2 * n2;
    return 560 * n4 - 120 * n2 + 3;
}
