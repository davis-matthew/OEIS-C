/*
 * A105942  a(n) = binomial(n+6,6)*binomial(n+9,6).
 * https://oeis.org/A105942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+6,6) * binomial(n+9,6)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 14, u64 n <= 115, u128 n <= 1000,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t t1 = binom(n + 6, 6);
    seq_t t2 = binom(n + 9, 6);
    return t1 * t2;
}
