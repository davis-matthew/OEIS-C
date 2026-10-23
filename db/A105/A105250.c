/*
 * A105250  a(n) = binomial(n+3,n)*binomial(n+7,n).
 * https://oeis.org/A105250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+3, n) * binomial(n+7, n)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 22, u64 n <= 233, u128 n <= 1000,
 *            big any n
 * Helpers:   binom_nk
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_nk.h"

seq_t a(idx_t n)
{
    seq_t c1 = binom_nk(n + 3, 3);
    seq_t c2 = binom_nk(n + 7, 7);
    return c1 * c2;
}
