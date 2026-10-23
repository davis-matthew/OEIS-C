/*
 * A033336  a(n) = floor(16/n).
 * https://oeis.org/A033336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(16/n)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Return the integer division of 16 by n, truncated towards zero.
       Since both operands are unsigned, the result is floor(16/n). */
    return (seq_t)16 / n;
}
