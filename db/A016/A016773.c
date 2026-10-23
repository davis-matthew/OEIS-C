/*
 * A016773  a(n) = (3*n)^9.
 * https://oeis.org/A016773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n)^9
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 46, u128 n <= 1000,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n;
    base *= 3;
    return big_pow(base, 9u);
}
