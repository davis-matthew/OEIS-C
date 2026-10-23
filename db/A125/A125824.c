/*
 * A125824  Denominator of n!/3^n.
 * https://oeis.org/A125824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 3^(n - sum_{k>=1} floor(n/3^k))
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 37, u64 n <= 75, u128 n <= 154,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    idx_t e = 0;
    idx_t t = n;
    while (t) {
        t /= 3;
        e += t;
    }
    idx_t exp = n - e;   /* exponent of 3 in the denominator */
    return pow3(exp);
}
