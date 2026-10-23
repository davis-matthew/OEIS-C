/*
 * A104476  a(n) = binomial(n+7,7)*binomial(n+11,7).
 * https://oeis.org/A104476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+7,7) * binomial(n+11,7)
 * Range:     u8 none, u16 n <= 2, u32 n <= 10, u64 n <= 74, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* compute C(n+7,7) and C(n+11,7) and multiply */
    seq_t b1 = binomial(n + 7, 7);
    seq_t b2 = binomial(n + 11, 7);
    return b1 * b2;
}
