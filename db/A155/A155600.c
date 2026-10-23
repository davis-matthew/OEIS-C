/*
 * A155600  a(n) = 9^n-2^n+1^n.
 * https://oeis.org/A155600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n - 2^n + 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t pow9 = seq_pow((seq_t)9, n);
    seq_t pow2 = seq_pow((seq_t)2, n);
    return pow9 - pow2 + (seq_t)1;
}
