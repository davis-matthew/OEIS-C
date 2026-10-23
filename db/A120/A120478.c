/*
 * A120478  a(n) = binomial(n+6,5) - binomial(n,5).
 * https://oeis.org/A120478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+6,5) - binomial(n,5)
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t n_plus_6 = n + 6;
    seq_t c1 = binomial(n_plus_6, 5);
    seq_t c2 = binomial(n, 5);
    return c1 - c2;
}
