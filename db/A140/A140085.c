/*
 * A140085  Period 8: repeat [0,1,1,2,1,2,2,3].
 * https://oeis.org/A140085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = repeat periodic pattern [0,1,1,2,1,2,2,3] with period 8
 *            (offset 0)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* static lookup table for one full period */
    static const seq_t values[8] = { (seq_t)0, (seq_t)1, (seq_t)1, (seq_t)2,
                                     (seq_t)1, (seq_t)2, (seq_t)2, (seq_t)3 };

    /* compute index modulo 8 using efficient bitwise operation */
    size_t i = (size_t)(n & (idx_t)7);   /* n % 8, works for unsigned n */

    return values[i];
}
