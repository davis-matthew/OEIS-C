/*
 * A081682  a(n) = (9^n - 8^n - 7^n - 6^n + 4*5^n)/2.
 * https://oeis.org/A081682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (9^n - 8^n - 7^n - 6^n + 4*5^n)/2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t exp = (seq_t)n;
    seq_t pow5 = big_pow((seq_t)5, exp);
    seq_t pow6 = big_pow((seq_t)6, exp);
    seq_t pow7 = big_pow((seq_t)7, exp);
    seq_t pow8 = big_pow((seq_t)8, exp);
    seq_t pow9 = big_pow((seq_t)9, exp);

    seq_t numerator = pow9 - pow8 - pow7 - pow6 + (seq_t)4 * pow5;
    return numerator / (seq_t)2;
}
