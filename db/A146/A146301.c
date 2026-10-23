/*
 * A146301  a(n) = (8*n+3)*(8*n+7).
 * https://oeis.org/A146301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n+3)*(8*n+7)
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t t1 = 8 * x + 3;
    seq_t t2 = 8 * x + 7;
    return t1 * t2;
}
