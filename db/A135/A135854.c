/*
 * A135854  a(n) = (n+1)*(2^n+1) for n > 0 with a(0)=1.
 * https://oeis.org/A135854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(2^n+1)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t pow2n = pow2(n);      /* 2^n */
    pow2n += (seq_t)1;          /* 2^n + 1 */
    seq_t nplus1 = (seq_t)n + (seq_t)1; /* n + 1 */
    return nplus1 * pow2n;
}
