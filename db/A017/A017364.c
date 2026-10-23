/*
 * A017364  a(n) = (10*n + 7)^12.
 * https://oeis.org/A017364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 7)^12
 * Range:     u8 none, u16 none, u32 none, u64 n <= 3, u128 n <= 161,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t base = ((seq_t)n) * 10 + 7;
    return seq_pow(base, (idx_t)12);
}
