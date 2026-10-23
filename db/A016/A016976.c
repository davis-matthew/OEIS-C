/*
 * A016976  a(n) = (6*n + 5)^8.
 * https://oeis.org/A016976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n+5)^8
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 41, u128 n <= 1000,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)6 * (seq_t)n + (seq_t)5;
    return big_pow(base, (seq_t)8);
}
