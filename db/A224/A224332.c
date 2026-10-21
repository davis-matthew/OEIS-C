/*
 * A224332  Number of idempotent n X n 0..7 matrices of rank n-1.
 * https://oeis.org/A224332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*8^(n-1)-1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1, but guard */
    seq_t pow8 = pow_int((seq_t)8, n - 1); /* 8^(n-1) */
    seq_t tmp  = (seq_t)2 * pow8 - (seq_t)1;
    return (seq_t)n * tmp;
}
