/*
 * A170938  a(n) = 4^n+2^n+2.
 * https://oeis.org/A170938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n + 2^n + 2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t pow4 = seq_pow((seq_t)4, n);
    seq_t pow2 = seq_pow((seq_t)2, n);
    return pow4 + pow2 + (seq_t)2;
}
