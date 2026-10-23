/*
 * A017148  a(n) = (8*n+6)^12.
 * https://oeis.org/A017148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n+6)^12
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 4, u128 n <= 202,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)8 * n + 6;
    seq_t exp  = 12;
    return big_pow(base, exp);
}
