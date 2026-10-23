/*
 * A167280  Period 12: repeat [0, 0, 1, 2, 4, 7, 4, 8, 7, 4, 8, 5].
 * https://oeis.org/A167280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = period[ n mod 12 ] where period =
 *            {0,0,1,2,4,7,4,8,7,4,8,5}
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t period[12] = {0, 0, 1, 2, 4, 7, 4, 8, 7, 4, 8, 5};
    return period[(size_t)(n % 12)];
}
