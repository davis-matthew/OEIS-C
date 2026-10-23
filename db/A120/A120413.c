/*
 * A120413  Largest even number strictly less than n^2.
 * https://oeis.org/A120413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2 - 2 for even n; a(n) = n^2 - 1 for odd n
 * Range:     u8 n <= 16, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = nn * nn;
    if ((n & 1ULL) == 0)
        val -= 2;
    else
        val -= 1;
    return val;
}
