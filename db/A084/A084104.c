/*
 * A084104  A period 6 sequence.
 * https://oeis.org/A084104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n - 6) for n >= 6; initial terms: a(0)=1, a(1)=4,
 *            a(2)=7, a(3)=7, a(4)=4, a(5)=1
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    static const seq_t pattern[6] = {1, 4, 7, 7, 4, 1};
    return pattern[(size_t)(n % 6)];
}
