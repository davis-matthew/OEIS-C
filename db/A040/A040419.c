/*
 * A040419  Continued fraction for sqrt(440).
 * https://oeis.org/A040419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=20; for n>0, a(n)=40 if n is even, 1 if n is odd
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)20;
    /* n is unsigned, so n & 1 selects least significant bit */
    if ((n & 1ULL) == 0)
        return (seq_t)40;
    return (seq_t)1;
}
