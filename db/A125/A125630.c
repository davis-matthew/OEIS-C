/*
 * A125630  a(n) is the number of integers k less than 10^n such that the
 *          decimal representation of k lacks the digit 1 and at least one of
 *          digits 2,3,4,5,6,7,8,9.
 * https://oeis.org/A125630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 8*8^n-28*7^n+56*6^n-70*5^n+56*4^n-28*3^n+8*2^n-1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;   /* offset: a(1) is the first term */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n);
    seq_t pow5 = big_pow((seq_t)5, (seq_t)n);
    seq_t pow6 = big_pow((seq_t)6, (seq_t)n);
    seq_t pow7 = big_pow((seq_t)7, (seq_t)n);
    seq_t pow8 = big_pow((seq_t)8, (seq_t)n);

    seq_t sum_pos = (seq_t)8 * pow8;
    sum_pos += (seq_t)56 * pow6;
    sum_pos += (seq_t)56 * pow4;
    sum_pos += (seq_t)8 * pow2;

    seq_t sum_neg = (seq_t)28 * pow7;
    sum_neg += (seq_t)70 * pow5;
    sum_neg += (seq_t)28 * pow3;

    seq_t result = sum_pos;
    result -= sum_neg;
    result -= (seq_t)1;

    return result;
}
