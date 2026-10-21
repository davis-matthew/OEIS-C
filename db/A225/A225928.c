/*
 * A225928  a(n) = 4*16^n + 8*4^n + 17.
 * https://oeis.org/A225928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*16^n + 8*4^n + 17 Equivalent: a(n) = 2^(4n+2) +
 *            2^(2n+3) + 17
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* exponent for 16^n term: 4n + 2  (since 4*16^n = 2^(4n+2)) */
    seq_t exp1 = ((seq_t)n << 2) + 2;
    /* exponent for 4^n term:   2n + 3  (since 8*4^n = 2^(2n+3)) */
    seq_t exp2 = ((seq_t)n << 1) + 3;

    seq_t term1 = big_pow((seq_t)2, exp1);
    seq_t term2 = big_pow((seq_t)2, exp2);

    return term1 + term2 + 17;
}
