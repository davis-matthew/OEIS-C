/*
 * A178896  a(n) = n OR 9n, where OR is bitwise OR.
 * https://oeis.org/A178896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n | 9n
 * Range:     u8 n <= 28, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn | (nn * 9);
}
