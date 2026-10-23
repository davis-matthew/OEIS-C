/*
 * A128989  a(n) = (n^3 - n^2)*6^n.
 * https://oeis.org/A128989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 - n^2) * 6^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 19, u128 n <= 43,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t N = n;                     /* cast index to seq_t */
    seq_t pow6 = pow_uint(6, n);      /* 6^n */
    seq_t val = N * N;                /* n^2 */
    val *= (N - 1);                   /* * (n-1) */
    val *= pow6;                      /* * 6^n */
    return val;
}
