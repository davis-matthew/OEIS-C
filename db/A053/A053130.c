/*
 * A053130  Binomial coefficients C(2*n-7,8).
 * https://oeis.org/A053130
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n-7,8)
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 28, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 8)
        return 0;
    idx_t m = 2*n - 7;
    return binom(m, 8);
}
