/*
 * A017457  a(n) = (11*n + 5)^9.
 * https://oeis.org/A017457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 5)^9
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 12, u128 n <= 1737,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)(11 * n + 5);
    return big_pow(base, (seq_t)9);
}
