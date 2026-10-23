/*
 * A175976  a(n) = 4^n - 3*n + 1.
 * https://oeis.org/A175976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - 3*n + 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t pow4 = seq_pow((seq_t)4, n);
    seq_t tmp  = (seq_t)3 * (seq_t)n;
    return pow4 - tmp + (seq_t)1;
}
