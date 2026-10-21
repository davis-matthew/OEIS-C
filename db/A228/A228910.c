/*
 * A228910  a(n) = 8^n - 7*7^n + 21*6^n - 35*5^n + 35*4^n - 21*3^n + 7*2^n -
 *          1.
 * https://oeis.org/A228910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n - 7*7^n + 21*6^n - 35*5^n + 35*4^n - 21*3^n + 7*2^n -
 *            1
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t p2 = seq_pow((seq_t)2, n);
    seq_t p3 = seq_pow((seq_t)3, n);
    seq_t p4 = seq_pow((seq_t)4, n);
    seq_t p5 = seq_pow((seq_t)5, n);
    seq_t p6 = seq_pow((seq_t)6, n);
    seq_t p7 = seq_pow((seq_t)7, n);
    seq_t p8 = seq_pow((seq_t)8, n);

    seq_t res = p8
        - (seq_t)7 * p7
        + (seq_t)21 * p6
        - (seq_t)35 * p5
        + (seq_t)35 * p4
        - (seq_t)21 * p3
        + (seq_t)7 * p2
        - (seq_t)1;

    return res;
}
