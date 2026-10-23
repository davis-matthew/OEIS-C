/*
 * A011851  a(n) = floor(binomial(n,5)/5).
 * https://oeis.org/A011851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(binomial(n,5)/5)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 165, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 5) return 0;
    seq_t b = binomial(n, 5);
    return b / 5;
}
