/*
 * A040093  Continued fraction for sqrt(104).
 * https://oeis.org/A040093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(104):
   a(0) = 10
   For n ≥ 1: a(n) = 5 if n is odd, 20 if n is even. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)10;
    /* n is idx_t (unsigned); use bitwise or modulo to get parity. */
    return (seq_t)((n & 1U) ? 5 : 20);
}
