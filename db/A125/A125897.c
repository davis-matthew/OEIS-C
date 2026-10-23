/*
 * A125897  a(n) is the number of nonnegative integers k less than 10^n such
 *          that the decimal representation of k lacks at least one of the
 *          digits 1,2,3 and at least one of the digits 4,5,6,7,8,9.
 * https://oeis.org/A125897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 18*8^n-63*7^n+111*6^n-120*5^n+83*4^n-36*3^n+9*2^n-1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pow8 = big_pow((seq_t)8, (seq_t)n);
    seq_t pow7 = big_pow((seq_t)7, (seq_t)n);
    seq_t pow6 = big_pow((seq_t)6, (seq_t)n);
    seq_t pow5 = big_pow((seq_t)5, (seq_t)n);
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n);
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);

    seq_t result = (seq_t)18 * pow8 - (seq_t)63 * pow7;
    result += (seq_t)111 * pow6;
    result -= (seq_t)120 * pow5;
    result += (seq_t)83 * pow4;
    result -= (seq_t)36 * pow3;
    result += (seq_t)9 * pow2;
    result -= (seq_t)1;
    return result;
}
