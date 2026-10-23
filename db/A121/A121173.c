/*
 * A121173  Sequence S with property that for n in S, a(n) = a(1) + a(2) +...+
 *          a(n-1) and for n not in S, a(n) = n+1.
 * https://oeis.org/A121173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n+1 for odd n, otherwise * a(n) = 2*(2^(n/2+1)-2)-n for
 *            even n
 * Range:     u8 n <= 13, u16 n <= 29, u32 n <= 61, u64 n <= 125,
 *            u128 n <= 253, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n & 1U) {
        /* odd n */
        return (seq_t)n + 1;
    } else {
        /* even n */
        idx_t e = (n >> 1) + 1;     /* exponent: n/2 + 1 */
        seq_t p  = pow2(e);        /* 2^e */
        seq_t two = (seq_t)2;
        return two * (p - two) - (seq_t)n;
    }
}
