/*
 * A027803  a(n) = 35*(n+1)*binomial(n+4, 7)/4.
 * https://oeis.org/A027803
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+1,4)*C(n+4,4)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 34, u64 n <= 565, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;
    seq_t t1 = binomial(n + 1, 4);
    seq_t t2 = binomial(n + 4, 4);
    return t1 * t2;
}
