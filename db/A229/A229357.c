/*
 * A229357  Total sum of n-th powers of parts in all partitions of 6.
 * https://oeis.org/A229357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19 + 8*2^n + 4*3^n + 2*4^n + 5^n + 6^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n);
    seq_t pow5 = big_pow((seq_t)5, (seq_t)n);
    seq_t pow6 = big_pow((seq_t)6, (seq_t)n);

    seq_t res = (seq_t)19;
    res += (seq_t)8 * pow2;
    res += (seq_t)4 * pow3;
    res += (seq_t)2 * pow4;
    res += pow5;
    res += pow6;

    return res;
}
