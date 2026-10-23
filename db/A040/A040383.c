/*
 * A040383  Continued fraction for sqrt(404).
 * https://oeis.org/A040383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 20; for n ≥ 1, a(n) = 10 if n is odd, 40 if n is even
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)20;
    return (n & 1U) ? (seq_t)10 : (seq_t)40;
}
