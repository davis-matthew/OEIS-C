/*
 * A155623  a(n) = 11^n + 3^n - 1.
 * https://oeis.org/A155623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n + 3^n - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (also satisfies a(n) = 14*a(n-1) - 33*a(n-2) - 20 for n>1, a(0)=1, a(1)=13) */

#include "../utils/big_pow.h"   /* provides seq_t big_pow(seq_t base, seq_t exp) */

seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t pow_11 = big_pow((seq_t)11, (seq_t)n);
    seq_t pow_3  = big_pow((seq_t)3,  (seq_t)n);

    return pow_11 + pow_3 - (seq_t)1;
}
