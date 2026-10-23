/*
 * A156227  Period 12: repeat [0,1,3,8,3,1,0,8,6,1,6,8].
 * https://oeis.org/A156227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-12), with period [0,1,3,8,3,1,0,8,6,1,6,8]
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    static const seq_t period[12] = {0, 1, 3, 8, 3, 1, 0, 8, 6, 1, 6, 8};
    return period[(size_t)(n % 12)];
}
