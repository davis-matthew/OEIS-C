/*
 * A016724  Expansion of (2-2*x-x^2)/((1-2*x^2)*(1-x)^2).
 * https://oeis.org/A016724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*n) = 2^n + 2*n + 1, a(2*n+1) = 2*n + 2
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 255, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {                /* n is even: a(2*m) */
        idx_t m = n >> 1;              /* m = n/2 */
        seq_t part = pow2(m);           /* part = 2^m */
        return part + (seq_t)(2 * m) + (seq_t)1;
    } else {                           /* n is odd: a(2*m+1) */
        idx_t m = (n - 1) >> 1;        /* m = (n-1)/2 */
        return (seq_t)(2 * m) + (seq_t)2;
    }
}
