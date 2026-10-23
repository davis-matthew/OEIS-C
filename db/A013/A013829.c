/*
 * A013829  a(n) = 3^(5*n + 4).
 * https://oeis.org/A013829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(5*n + 4)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    idx_t exp = 5*n + 4;
    return seq_pow((seq_t)3, exp);
}
