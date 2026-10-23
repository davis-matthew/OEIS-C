/*
 * A193578  a(n) = (13^n + 1)/2.
 * https://oeis.org/A193578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (13^n + 1)/2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 13^n using the provided helper, then evaluate (13^n + 1)/2. */
    seq_t pow = big_pow((seq_t)13, (seq_t)n);
    return (pow + 1) / 2;
}
