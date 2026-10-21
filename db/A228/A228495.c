/*
 * A228495  Characteristic function of the odd odious numbers (A092246).
 * https://oeis.org/A228495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if n is odd and the binary weight of n is odd, else 0
 * Range:     u8 n <= 9508, u16 n <= 9508, u32 n <= 9508, u64 n <= 9508,
 *            u128 n <= 9508, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Odd numbers only */
    if ((n & 1) == 0)
        return (seq_t)0;

    /* Parity of popcount – use XOR to keep intermediate small */
    idx_t v = n;
    unsigned int parity = 0;   /* 0 or 1 */

    while (v != 0) {
        parity ^= (unsigned int)(v & 1);
        v >>= 1;
    }

    return (seq_t)parity;
}
