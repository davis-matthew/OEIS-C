/*
 * A158068  Period 6: repeat [1, 2, 2, 1, 5, 5].
 * https://oeis.org/A158068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = pattern[n mod 6], pattern = {1,2,2,1,5,5}
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    static const seq_t pattern[6] = {1, 2, 2, 1, 5, 5};
    return pattern[(size_t)(n % 6)];
}
