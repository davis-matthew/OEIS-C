/*
 * A155605  a(n) = 7^n+3^n-1.
 * https://oeis.org/A155605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n + 3^n - 1, offset: 0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t p7 = seq_pow((seq_t)7, n);
    seq_t p3 = seq_pow((seq_t)3, n);
    return p7 + p3 - (seq_t)1;
}
