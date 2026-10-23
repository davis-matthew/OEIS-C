/*
 * A028038  Expansion of 1/((1-3x)(1-4x)(1-7x)(1-8x)).
 * https://oeis.org/A028038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*8^(n+3)-5*7^(n+3)+5*4^(n+3)-3^(n+4))/60
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t tot_exp = (seq_t)(n + 3);
    seq_t term1 = big_pow((seq_t)8, tot_exp) * (seq_t)3;
    seq_t term2 = big_pow((seq_t)7, tot_exp) * (seq_t)5;
    seq_t term3 = big_pow((seq_t)4, tot_exp) * (seq_t)5;
    seq_t term4 = big_pow((seq_t)3, (seq_t)(n + 4));

    seq_t num = term1 + term3;
    num -= term2;
    num -= term4;

    return num / (seq_t)60;
}
