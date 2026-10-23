/*
 * A183647  Number of (n+1) X 5 0..4 arrays with every 2 X 2 subblock summing
 *          to 8.
 * https://oeis.org/A183647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*5^n + 120*4^n + 540*3^n + 1140*2^n + 1320
 * Range:     u8 none, u16 n <= 3, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 1 (offset 1). */
    if (n == 0)
        return (seq_t)0;

    seq_t pow5 = big_pow((seq_t)5, (seq_t)n);
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n);
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);

    seq_t res = 5 * pow5;
    res += 120 * pow4;
    res += 540 * pow3;
    res += 1140 * pow2;
    res += 1320;
    return res;
}
