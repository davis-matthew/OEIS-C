/*
 * A135162  a(n) = 7^n - 5^n - 3^n + 2^n. Constants are the prime numbers in
 *          decreasing order.
 * https://oeis.org/A135162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n - 5^n - 3^n + 2^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"  /* assumes pow_seq(seq_t, int) is provided */

seq_t a(idx_t n)
{
    seq_t p7 = pow_seq((seq_t)7, (int)n);
    seq_t p5 = pow_seq((seq_t)5, (int)n);
    seq_t p3 = pow_seq((seq_t)3, (int)n);
    seq_t p2 = pow_seq((seq_t)2, (int)n);
    return p7 - p5 - p3 + p2;
}
