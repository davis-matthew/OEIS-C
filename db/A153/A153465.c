/*
 * A153465  a(n) = 9*4^n - 2.
 * https://oeis.org/A153465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*4^n - 2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t pow4 = seq_pow((seq_t)4, n);
    return (seq_t)9 * pow4 - 2;
}
