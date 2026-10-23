/*
 * A175806  a(n) = 27*2^n.
 * https://oeis.org/A175806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27 * 2^n
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t v = seq_pow((seq_t)2, n); /* compute 2^n */
    v *= (seq_t)27;                /* multiply by 27 */
    return v;
}
