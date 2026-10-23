/*
 * A107319  a(n) = C(n+8,8) * C(n+6,6).
 * https://oeis.org/A107319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binom(n+8, 8) * binom(n+6, 6)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 77, u128 n <= 1000,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t b1 = binom(n + 8, 8);
    seq_t b2 = binom(n + 6, 6);
    return b1 * b2;
}
