/*
 * A168185  Characteristic function of numbers that are not multiples of 12.
 * https://oeis.org/A168185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n mod 12 == 0, otherwise 1
 * Range:     u8 n <= 8922, u16 n <= 8922, u32 n <= 8922, u64 n <= 8922,
 *            u128 n <= 8922, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    return (n % 12 == 0) ? (seq_t)0 : (seq_t)1;
}
