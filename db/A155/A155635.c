/*
 * A155635  a(n) = 8^n+5^n-1.
 * https://oeis.org/A155635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n + 5^n - 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t exp = (seq_t)n;
    seq_t p8  = big_pow((seq_t)8, exp);
    seq_t p5  = big_pow((seq_t)5, exp);
    seq_t res = p8 + p5 - (seq_t)1;
    return res;
}
