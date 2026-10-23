/*
 * A021186  Decimal expansion of 1/182.
 * https://oeis.org/A021186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A021186: a(n) = 0 for n<2; otherwise a(n) = [5,4,9,4,5,0][(n-2)%6] */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < (idx_t)2) return (seq_t)0;
    static const unsigned char pat[6] = {5, 4, 9, 4, 5, 0};
    idx_t i = (n - (idx_t)2) % (idx_t)6;
    return (seq_t)pat[(size_t)i];
}
