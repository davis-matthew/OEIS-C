/*
 * A017530  a(n) = (12*n)^10.
 * https://oeis.org/A017530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n)^10
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 7, u128 n <= 594,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * (seq_t)12;
    return big_pow(base, 10);
}
