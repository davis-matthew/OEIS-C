/*
 * A209261  a(n) = n^13 + 13*n + 13^n.
 * https://oeis.org/A209261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^13 + 13*n + 13^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t base_n = (seq_t)n;
    seq_t n13 = base_n;
    for (idx_t i = 1; i < 13; ++i) {
        n13 *= base_n;
    }
    seq_t term13 = (seq_t)13 * base_n;
    seq_t pow13  = big_pow((seq_t)13, (seq_t)n);
    seq_t result = n13;
    result += term13;
    result += pow13;
    return result;
}
