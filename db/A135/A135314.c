/*
 * A135314  a(n) = 7^n*6^(n^2).
 * https://oeis.org/A135314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7^n*6^(n^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t power7 = big_pow((seq_t)7, (seq_t)n);
    seq_t n_sq   = (seq_t)n * (seq_t)n;
    seq_t power6 = big_pow((seq_t)6, n_sq);
    return power7 * power6;
}
