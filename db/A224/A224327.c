/*
 * A224327  Number of idempotent n X n 0..2 matrices of rank n-1.
 * https://oeis.org/A224327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*3^(n-1)-1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pow3 = big_pow((seq_t)3, (seq_t)(n - 1));
    seq_t tmp  = (seq_t)2 * pow3 - (seq_t)1;
    return (seq_t)n * tmp;
}
