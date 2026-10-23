/*
 * A171677  Period 9: repeat [7, 5, 7, 4, 2, 4, 1, 8, 1].
 * https://oeis.org/A171677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = periodic sequence with period 9: [7, 5, 7, 4, 2, 4, 1, 8,
 *            1]
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t period[9] = {7, 5, 7, 4, 2, 4, 1, 8, 1};
    size_t idx = (size_t)(n % 9);
    return period[idx];
}
