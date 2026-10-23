/*
 * A135349  a(n) = 6^n*5^(n^2).
 * https://oeis.org/A135349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6^n*5^(n^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t pow6 = seq_pow((seq_t)6, n);
    seq_t pow5 = seq_pow((seq_t)5, n * n);
    return seq_mul(pow6, pow5);
}
