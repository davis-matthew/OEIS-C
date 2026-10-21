/*
 * A212126  Period 13: repeat (0,0,1,0,0,1,0,1,0,0,1,0,1).
 * https://oeis.org/A212126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = pattern[ n mod 13 ], pattern =
 *            {0,0,1,0,0,1,0,1,0,0,1,0,1}
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
static const seq_t pattern[13] = {0,0,1,0,0,1,0,1,0,0,1,0,1};
seq_t a(idx_t n) {
    size_t idx = (size_t)(n % 13U);
    return pattern[idx];
}
