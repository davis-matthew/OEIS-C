/*
 * A131951  a(n) = 2^n + n*(n+3).
 * https://oeis.org/A131951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + n*(n+3)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t val = pow2(n);                     /* 2^n */
    seq_t add = (seq_t)n * ((seq_t)n + (seq_t)3); /* n*(n+3) */
    val += add;
    return val;
}
