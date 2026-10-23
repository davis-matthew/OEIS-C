/*
 * A152855  Period 5: Repeat [1, 2, 0, 2, 0].
 * https://oeis.org/A152855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = [1, 2, 0, 2, 0][n mod 5]
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>


seq_t a(idx_t n)
{
    static const seq_t pattern[5] = {1, 2, 0, 2, 0};
    return pattern[(size_t)(n % 5)];
}
