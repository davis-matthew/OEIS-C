/*
 * A011852  a(n) = floor(binomial(n,6)/6).
 * https://oeis.org/A011852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(binomial(n,6)/6)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 164, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    return binomial(n, 6) / (seq_t)6;
}
