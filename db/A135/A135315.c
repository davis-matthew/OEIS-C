/*
 * A135315  a(n) = 8^n * 7^(n^2).
 * https://oeis.org/A135315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n * 7^(n^2)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t pow8 = seq_pow((seq_t)8, n);
    idx_t nsq = n * n;
    seq_t pow7 = seq_pow((seq_t)7, nsq);
    return seq_mul(pow8, pow7);
}
