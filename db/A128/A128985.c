/*
 * A128985  a(n) = (n^3 - n^2)*2^n.
 * https://oeis.org/A128985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 - n^2)*2^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 19, u64 n <= 47, u128 n <= 107,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n >= 1 for the sequence; 0 is outside the offset */
    if (n == 0) return 0;

    seq_t t   = (seq_t)n;          /* cast index to seq_t */
    seq_t val = t * (t - 1) * t;   /* n^2 * (n-1) = n^3 - n^2 */
    seq_t p   = pow2(n);           /* 2^n */

    return val * p;
}
