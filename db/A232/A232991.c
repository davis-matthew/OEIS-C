/*
 * A232991  Period 6: repeat [1, 0, 0, 0, 1, 0].
 * https://oeis.org/A232991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n mod 6 == 0 or n mod 6 == 4, otherwise 0
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t r = n % 6;     /* compute n modulo 6 */
    if (r == 0 || r == 4)
        return (seq_t)1;
    return (seq_t)0;
}
