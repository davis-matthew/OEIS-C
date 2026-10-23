/*
 * A155663  a(n) = 9^n + 8^n - 1.
 * https://oeis.org/A155663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n + 8^n - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    seq_t pow9 = (seq_t)1;
    seq_t pow8 = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        pow9 = mul_small(pow9, 9);
    for (idx_t i = 1; i <= n; ++i)
        pow8 = mul_small(pow8, 8);
    return (pow9 + pow8) - (seq_t)1;
}
