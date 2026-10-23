/*
 * A013781  a(n) = 4^(4*n + 3).
 * https://oeis.org/A013781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(4*n+3)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t exp = 4 * (seq_t)n + 3;
    return big_pow((seq_t)4, exp);
}
