/*
 * A164090  a(n) = 2*a(n-2) for n > 2; a(1) = 2, a(2) = 3.
 * https://oeis.org/A164090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) for n > 2; a(1) = 2, a(2) = 3
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 62, u64 n <= 126,
 *            u128 n <= 254, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    if (n & 1) {
        /* odd indices: a(n) = 2^((n+1)/2) */
        idx_t exp = (n + 1) >> 1;
        return pow_uint(2, exp);
    } else {
        /* even indices: a(n) = 3 * 2^((n/2) - 1) */
        idx_t exp = (n >> 1) - 1;
        seq_t p = pow_uint(2, exp);
        return (seq_t)3 * p;
    }
}
