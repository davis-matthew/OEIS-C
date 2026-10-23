/*
 * A183665  Number of (n+1) X 3 0..6 arrays with every 2 X 2 subblock summing
 *          to 12.
 * https://oeis.org/A183665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*7^n + 36*6^n + 60*5^n + 72*4^n + 72*3^n + 60*2^n + 36
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute all required powers of 2..7 */
    seq_t p7 = big_pow((seq_t)7, (seq_t)n);
    seq_t p6 = big_pow((seq_t)6, (seq_t)n);
    seq_t p5 = big_pow((seq_t)5, (seq_t)n);
    seq_t p4 = big_pow((seq_t)4, (seq_t)n);
    seq_t p3 = big_pow((seq_t)3, (seq_t)n);
    seq_t p2 = big_pow((seq_t)2, (seq_t)n);

    /* Combine according to the formula */
    seq_t result = 7 * p7
                 + 36 * p6
                 + 60 * p5
                 + 72 * p4
                 + 72 * p3
                 + 60 * p2
                 + 36;

    return result;
}
