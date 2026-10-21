/*
 * A229666  Number of defective 4-colorings of an n X 2 0..3 array connected
 *          horizontally, antidiagonally and vertically with exactly two
 *          mistakes, and colors introduced in row-major 0..3 order.
 * https://oeis.org/A229666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=4, a(3)=61, a(n)=2^(2*n-7)*(106-145*n+50*n^2) for
 *            n>3 (equivalently a(n)=12*a(n-1)-48*a(n-2)+64*a(n-3) for n>6)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 58,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)61;

    /* compute 2^(2*n-7) */
    idx_t exp = 2*n - 7;
    seq_t pow = pow2(exp);

    /* compute (106 - 145*n + 50*n^2) */
    seq_t nn  = (seq_t)n;
    seq_t poly = (seq_t)106
                 - (seq_t)145 * nn
                 + (seq_t)50 * nn * nn;

    return pow * poly;
}
