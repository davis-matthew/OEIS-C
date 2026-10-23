/*
 * A017146  a(n) = (8*n + 6)^10.
 * https://oeis.org/A017146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 6)^10
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 9, u128 n <= 890,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)8 * (seq_t)n + (seq_t)6;
    return seq_pow(base, (idx_t)10);
}
