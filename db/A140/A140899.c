/*
 * A140899  Period 10: repeat [7, 9, 5, 1, 3, 3, 1, 5, 9, 7].
 * https://oeis.org/A140899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-10), period 10: [7, 9, 5, 1, 3, 3, 1, 5, 9, 7]
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    static const seq_t period[10] = {7, 9, 5, 1, 3, 3, 1, 5, 9, 7};
    return period[(size_t)(n % 10)];
}
