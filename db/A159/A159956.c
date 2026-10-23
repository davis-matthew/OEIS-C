/*
 * A159956  Period 16 : 0,1,2,3,1,2,3,0,2,3,0,1,3,0,1,2 .
 * https://oeis.org/A159956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n%16) with period 16: 0,1,2,3,1,2,3,0,2,3,0,1,3,0,1,2
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t table[16] = {
        0, 1, 2, 3,
        1, 2, 3, 0,
        2, 3, 0, 1,
        3, 0, 1, 2
    };
    return table[(size_t)(n & 15)];
}
