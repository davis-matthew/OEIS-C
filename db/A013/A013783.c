/*
 * A013783  a(n) = 5^(4*n + 3).
 * https://oeis.org/A013783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(4*n + 3)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    idx_t exp = 4*n + 3;
    return seq_pow((seq_t)5, exp);
}
