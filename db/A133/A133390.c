/*
 * A133390  Period 18: repeat 1, 4, 7, 2, 2, 5, 4, 1, 1, 8, 5, 2, 7, 7, 4, 5,
 *          8, 8.
 * https://oeis.org/A133390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = repeat period of length 18:
 *            1,4,7,2,2,5,4,1,1,8,5,2,7,7,4,5,8,8
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t period[18] = {
        1, 4, 7, 2, 2, 5,
        4, 1, 1, 8, 5, 2,
        7, 7, 4, 5, 8, 8
    };
    return period[(size_t)(n % 18)];
}
