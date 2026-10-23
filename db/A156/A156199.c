/*
 * A156199  Period 12: repeat [1, 1, 4, 2, 4, 1, 1, 2, 7, 1, 7, 2].
 * https://oeis.org/A156199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) is periodic with period 12: [1,1,4,2,4,1,1,2,7,1,7,2]
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t period[12] = {
        1, 1, 4, 2, 4, 1, 1, 2, 7, 1, 7, 2
    };
    return period[(size_t)(n % 12)];
}
