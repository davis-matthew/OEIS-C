/*
 * A134965  a(1)=3, a(n) = a(n-1) + 7 + 2*mod(n-1, 2) for n>=2.
 * https://oeis.org/A134965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n - 4 if n is even * a(n) = 8*n - 5 if n is odd
 * Range:     u8 n <= 32, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t val = (seq_t)8 * n;
    if ((n & 1) == 0) {
        val -= (seq_t)4;
    } else {
        val -= (seq_t)5;
    }
    return val;
}
