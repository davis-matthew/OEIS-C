/*
 * A016904  a(n) = (5*n + 4)^8.
 * https://oeis.org/A016904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n + 4)^8
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 50, u128 n <= 1000,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 5U + 4;
    return seq_pow(base, 8);
}
