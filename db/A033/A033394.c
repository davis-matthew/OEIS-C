/*
 * A033394  a(n) = floor(74/n).
 * https://oeis.org/A033394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(74 / n)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. Guard against n == 0, though
       it is outside the intended domain. */
    if (n == 0)
        return 0;
    return (seq_t)74 / (seq_t)n;
}
