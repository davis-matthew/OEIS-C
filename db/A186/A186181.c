/*
 * A186181  Period 4 sequence [ 2, 2, 3, 2, ...] except a(0) = 1.
 * https://oeis.org/A186181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A186181
   a(0) = 1
   a(n) = 3 if n % 4 == 2
   a(n) = 2 otherwise (for n > 0)
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    /* n % 4 can be computed with a bitwise AND for speed,
       since 4 is a power of two. */
    if ((n & 3) == 2)
        return (seq_t)3;
    return (seq_t)2;
}
