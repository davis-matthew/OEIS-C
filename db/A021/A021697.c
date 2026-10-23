/*
 * A021697  Decimal expansion of 1/693.
 * https://oeis.org/A021697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = digit of the decimal expansion of 1/693 at index n. The
 *            expansion repeats the pattern 0,0,1,4,4,3
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pat[6] = {0, 0, 1, 4, 4, 3};
    return pat[(size_t)(n % 6)];
}
