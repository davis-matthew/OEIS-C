/*
 * A138199  a(n) = 14^(2*n+1) + 3^(2*n+1).
 * https://oeis.org/A138199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14^(2*n+1) + 3^(2*n+1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* exponent 2*n+1 (stored as seq_t to avoid overflow for large seq_t) */
    seq_t exp = (seq_t)n * (seq_t)2 + (seq_t)1;
    /* compute the powers with the provided big_pow helper */
    seq_t p14 = big_pow((seq_t)14, exp);
    seq_t p3  = big_pow((seq_t)3,  exp);
    /* sum the two terms and return */
    return p14 + p3;
}
