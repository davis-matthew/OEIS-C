/*
 * A016869  a(n) = (5n+1)^9.
 * https://oeis.org/A016869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5n+1)^9
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t base = ((seq_t)5 * n + 1);
    return seq_pow(base, 9);
}
