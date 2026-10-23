/*
 * A020538  a(n) = 4th Chebyshev polynomial (first kind) evaluated at 2^n.
 * https://oeis.org/A020538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*2^{4n} - 8*2^{2n} + 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t p2n = pow2(n);          /* 2^n                     */
    seq_t p4n = p2n * p2n;        /* 2^{2n} = (2^n)^2       */
    seq_t p8n = p4n * p4n;        /* 2^{4n} = (2^{2n})^2    */

    seq_t result = 8 * p8n;       /* 8 * 2^{4n}              */
    result -= 8 * p4n;            /* - 8 * 2^{2n}            */
    result += 1;                  /* + 1                     */
    return result;
}
