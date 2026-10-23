/*
 * A105946  a(n) = C(n+3,3) * C(n+5,5).
 * https://oeis.org/A105946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(n+3,3) * C(n+5,5)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 33, u64 n <= 580, u128 n <= 1000,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t c1 = binom(n + 3, 3);
    seq_t c2 = binom(n + 5, 5);
    return c1 * c2;
}
