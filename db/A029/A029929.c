/*
 * A029929  a(n) = n*(n + ceiling(2^n/12)).
 * https://oeis.org/A029929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * ( n + ceil(2^n / 12) ) = n * ( n + ( 2^n + 11 ) / 12
 *            )
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 61, u128 n <= 124,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;               /* n as seq_t */
    seq_t pow2n = pow2(n);              /* 2^n as seq_t */
    seq_t ceil_term = (pow2n + (seq_t)11) / (seq_t)12; /* ceil(2^n/12) */
    return nn * (nn + ceil_term);      /* final result */
}
