/*
 * A105251  a(n) = binomial(n+4,n)*binomial(n+8,n).
 * https://oeis.org/A105251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+4,4) * binomial(n+8,8)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binom_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_small.h"

seq_t a(idx_t n)
{
    /* binomial(n+4,4) = C(n+4,4) */
    seq_t b1 = binom_small(n + 4, 4);
    /* binomial(n+8,8) = C(n+8,8) */
    seq_t b2 = binom_small(n + 8, 8);
    return b1 * b2;
}
