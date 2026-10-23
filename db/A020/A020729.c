/*
 * A020729  Pisot sequences E(2,10), L(2,10), P(2,10), T(2,10).
 * https://oeis.org/A020729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*5^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t pow5 = big_pow((seq_t)5, (seq_t)n);
    return seq_mul((seq_t)2, pow5);
}
