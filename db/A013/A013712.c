/*
 * A013712  a(n) = 7^(2*n + 1).
 * https://oeis.org/A013712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^(2*n + 1)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t exp = ((seq_t)2 * (seq_t)n) + (seq_t)1;
    return big_pow((seq_t)7, exp);
}
