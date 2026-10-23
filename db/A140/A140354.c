/*
 * A140354  a(n) = binomial(n+9,9)*2^n.
 * https://oeis.org/A140354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+9,9)*2^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 34, u128 n <= 87,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t b = binomial(n + 9, (idx_t)9);
    return b * pow2(n);
}
