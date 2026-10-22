/*
 * A005587  a(n) = n*(n+5)*(n+6)*(n+7)/24.
 * https://oeis.org/A005587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(7+n, 4) - C(7+n, 3)
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 441, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t m = 7 + n;
    return binom(m, 4) - binom(m, 3);
}
