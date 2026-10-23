/*
 * A104680  a(n) = binomial(n+7,7)*binomial(n+12,7).
 * https://oeis.org/A104680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binom(n+7,7) * binom(n+12,7)
 * Range:     u8 none, u16 n <= 1, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t t1 = binom(n + 7, 7);
    seq_t t2 = binom(n + 12, 7);
    return t1 * t2;
}
