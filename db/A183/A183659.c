/*
 * A183659  Number of (n+1) X 7 0..5 arrays with every 2 X 2 subblock summing
 *          to 10.
 * https://oeis.org/A183659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*6^n + 630*5^n + 7728*4^n + 36414*3^n + 95088*2^n +
 *            140070
 * Range:     u8 none, u16 none, u32 n <= 9, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t pow6 = seq_pow((seq_t)6, n);
    seq_t pow5 = seq_pow((seq_t)5, n);
    seq_t pow4 = seq_pow((seq_t)4, n);
    seq_t pow3 = seq_pow((seq_t)3, n);
    seq_t pow2 = seq_pow((seq_t)2, n);

    return (seq_t)6 * pow6
         + (seq_t)630 * pow5
         + (seq_t)7728 * pow4
         + (seq_t)36414 * pow3
         + (seq_t)95088 * pow2
         + (seq_t)140070;
}
