/*
 * A017131  a(n) = (8*n + 5)^7.
 * https://oeis.org/A017131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 5)^7
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 70, u128 n <= 10000,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t t = ((seq_t)n * 8) + 5;
    return big_pow(t, (seq_t)7);
}
