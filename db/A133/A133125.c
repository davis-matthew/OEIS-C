/*
 * A133125  a(n) = (7*3^n - (-3)^n)/6.
 * https://oeis.org/A133125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*3^n - (-3)^n)/6 Simplified: a(n) = 3^n if n is even
 *            a(n) = 4*3^(n-1) if n is odd
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Even n: return 3^n */
    if ((n & 1ULL) == 0)
        return pow3(n);

    /* Odd n: return 4 * 3^(n-1) */
    seq_t t = pow3(n-1);
    return t * ((seq_t)4);
}
