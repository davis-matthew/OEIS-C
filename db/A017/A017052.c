/*
 * A017052  a(n) = (7*n + 5)^12.
 * https://oeis.org/A017052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 5)^12
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 5, u128 n <= 231,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t base = ((seq_t)n * (seq_t)7) + (seq_t)5;
    return big_pow(base, (seq_t)12);
}
