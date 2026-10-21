/*
 * A092881  Number of ordered 3-multiantichains on an n-set.
 * https://oeis.org/A092881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n - 6*6^n + 6*5^n + 6*4^n - 12*3^n + 6*2^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   pow_const
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_const.h"

seq_t a(idx_t n)
{
    seq_t p8  = pow_const((seq_t)8,  n);
    seq_t p6  = pow_const((seq_t)6,  n);
    seq_t p5  = pow_const((seq_t)5,  n);
    seq_t p4  = pow_const((seq_t)4,  n);
    seq_t p3  = pow_const((seq_t)3,  n);
    seq_t p2  = pow_const((seq_t)2,  n);

    seq_t res = p8
                - (seq_t)6 * p6
                + (seq_t)6 * p5
                + (seq_t)6 * p4
                - (seq_t)12 * p3
                + (seq_t)6 * p2;
    return res;
}
