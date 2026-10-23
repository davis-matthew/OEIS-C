/*
 * A164278  a(n) = (6*n + 1)^(2*n) - 1.
 * https://oeis.org/A164278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (6*n + 1)^(2*n) - 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)6 * (seq_t)n + 1;
    seq_t exp = (seq_t)2 * (seq_t)n;
    seq_t res = big_pow(base, exp);
    return res - 1;
}
