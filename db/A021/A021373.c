/*
 * A021373  Decimal expansion of 1/369.
 * https://oeis.org/A021373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) is the nth decimal digit of 1/369, repeating pattern
 *            0,0,2,7,1
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    static const seq_t pat[5] = {0,0,2,7,1};
    return pat[(size_t)(n % 5)];
}
