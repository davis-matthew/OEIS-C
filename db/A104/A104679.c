/*
 * A104679  a(n) = C(n+5,5)*C(n+10,5).
 * https://oeis.org/A104679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(n+5,5)*C(n+10,5)
 * Range:     u8 none, u16 n <= 2, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    return binom(n + 5, 5) * binom(n + 10, 5);
}
