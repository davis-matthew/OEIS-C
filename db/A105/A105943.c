/*
 * A105943  a(n) = binomial(n+7,7) * binomial(n+10,7).
 * https://oeis.org/A105943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+7,7) * binomial(n+10,7)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 11, u64 n <= 74, u128 n <= 1000,
 *            big any n
 * Helpers:   binom7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom7.h"

seq_t a(idx_t n)
{
    /* n + 7 >= 7, n + 10 >= 10 always for n >= 0 */
    seq_t c1 = binom7(n + 7);
    seq_t c2 = binom7(n + 10);
    return c1 * c2;
}
