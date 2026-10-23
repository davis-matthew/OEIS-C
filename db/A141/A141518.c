/*
 * A141518  Period 5: repeat [1, 3, 5, 7, 9].
 * https://oeis.org/A141518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n mod 5)+1
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute (n mod 5) * 2 + 1 */
    seq_t r = (seq_t)n % 5;
    return r * 2 + 1;
}
