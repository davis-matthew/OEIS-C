/*
 * A183674  Number of (n+1) X 2 0..7 arrays with every 2 X 2 subblock summing
 *          to 14.
 * https://oeis.org/A183674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*8^n + 14*7^n + 12*6^n + 10*5^n + 8*4^n + 6*3^n + 4*2^n
 *            + 2
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t p2 = big_pow((seq_t)2, (seq_t)n);
    seq_t p3 = big_pow((seq_t)3, (seq_t)n);
    seq_t p4 = big_pow((seq_t)4, (seq_t)n);
    seq_t p5 = big_pow((seq_t)5, (seq_t)n);
    seq_t p6 = big_pow((seq_t)6, (seq_t)n);
    seq_t p7 = big_pow((seq_t)7, (seq_t)n);
    seq_t p8 = big_pow((seq_t)8, (seq_t)n);

    seq_t res = ((seq_t)8  * p8) + ((seq_t)14 * p7) + ((seq_t)12 * p6)
              + ((seq_t)10 * p5) + ((seq_t)8  * p4) + ((seq_t)6  * p3)
              + ((seq_t)4  * p2) + (seq_t)2;
    return res;
}
