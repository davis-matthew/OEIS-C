/*
 * A135273  Row 9 of A038207.
 * https://oeis.org/A135273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, fini, nonn, full
 * Formula:   a(n) = binomial(9,n) * 2^(9-n)
 * Range:     u8 none, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n > 9) return (seq_t)0;
    return binom(9, n) * pow2(9 - n);
}
