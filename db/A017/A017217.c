/*
 * A017217  a(n) = (9*n + 4)^9.
 * https://oeis.org/A017217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 4)^9
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 14, u128 n <= 2123,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t base = ((seq_t)9 * (seq_t)n) + (seq_t)4;
    return seq_pow(base, 9);
}
