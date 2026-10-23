/*
 * A017495  a(n) = (11*n + 8)^11.
 * https://oeis.org/A017495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 8)^11
 * Range:     u8 none, u16 none, u32 none, u64 n <= 4, u128 n <= 288,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)11 * (seq_t)n + (seq_t)8;
    return big_pow(base, (seq_t)11);
}
