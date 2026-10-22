/*
 * A000069  Odious numbers: numbers with an odd number of 1's in their binary
 *          expansion.
 * https://oeis.org/A000069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, core, nonn, nice, base
 * Formula:   a(n+1) = (1/2) * (4*n + 1 + (-1)^A000120(n)), i.e. a(n) =
 *            2*(n-1) + [A000120(n-1) is even]
 * Range:     u8 n <= 128, u16 n <= 9355, u32 n <= 9355, u64 n <= 9355,
 *            u128 n <= 9355, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m = n - 1;
    idx_t x = m;
    int parity = 0;
    while (x) {
        parity ^= (int)(x & 1);
        x >>= 1;
    }
    seq_t r = (seq_t)m;
    r = r * 2;
    if (!parity)
        r += 1;
    return r;
}
