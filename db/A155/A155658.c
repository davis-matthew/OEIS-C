/*
 * A155658  a(n) = 11^n + 7^n - 1.
 * https://oeis.org/A155658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n + 7^n - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pow11 = big_pow((seq_t)11, (seq_t)n);
    seq_t pow7  = big_pow((seq_t)7,  (seq_t)n);
    seq_t res   = pow11 + pow7;
    res -= (seq_t)1;
    return res;
}
