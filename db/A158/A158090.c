/*
 * A158090  Period 9: repeat [0, 6, 0, 6, 0, 0, 3, 3, 0].
 * https://oeis.org/A158090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = period[n mod 9], period = {0,6,0,6,0,0,3,3,0}
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t period[9] = {0, 6, 0, 6, 0, 0, 3, 3, 0};

seq_t a(idx_t n)
{
    return period[(size_t)(n % 9)];
}
