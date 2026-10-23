/*
 * A016185  a(n) = 9^n - 8^n.
 * https://oeis.org/A016185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n - 8^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t pow9 = seq_pow((seq_t)9, n);
    seq_t pow8 = seq_pow((seq_t)8, n);
    return pow9 - pow8;
}
