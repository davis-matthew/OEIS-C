/*
 * A086093  a(n) = 3^n + 2*n*4^(n-1).
 * https://oeis.org/A086093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = 3^n + 2*n*4^(n-1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
    seq_t pow4 = big_pow((seq_t)4, (seq_t)(n - 1));

    seq_t mul = (seq_t)(2 * n);
    seq_t term2 = mul * pow4;

    return pow3 + term2;
}
