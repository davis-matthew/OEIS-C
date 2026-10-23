/*
 * A140325  a(n) = binomial(n+8,8) * 2^n.
 * https://oeis.org/A140325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+8,8) * 2^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 36, u128 n <= 90,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t bin = binomial(n + 8, 8);
    seq_t two = pow2(n);
    return bin * two;
}
