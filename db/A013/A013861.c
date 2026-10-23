/*
 * A013861  a(n) = 11^(5*n+4).
 * https://oeis.org/A013861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^(5*n+4)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 6,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    idx_t exp = 5 * n + 4;
    seq_t base = 11;
    return seq_pow(base, exp);
}
