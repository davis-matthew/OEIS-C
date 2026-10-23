/*
 * A122219  Period 9: repeat 5, 4, 5, 4, 3, 4, 5, 4, 5.
 * https://oeis.org/A122219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = periodic pattern: 5,4,5,4,3,4,5,4,5 (period 9)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pattern[9] = {5, 4, 5, 4, 3, 4, 5, 4, 5};
    size_t idx = (size_t)(n % 9);
    return pattern[idx];
}
