/*
 * A135321  a(n) = 10^n * 9^(n^2).
 * https://oeis.org/A135321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n * 9^(n^2)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t pow10 = seq_pow((seq_t)10, n);
    idx_t exp9 = n * n;
    seq_t pow9 = seq_pow((seq_t)9, exp9);
    return seq_mul(pow10, pow9);
}
