/*
 * A225830  Number of binary pattern classes in the (6,n)-rectangular grid:
 *          two patterns are in the same class if one of them can be obtained
 *          by a reflection or 180-degree rotation of the other.
 * https://oeis.org/A225830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^(n-1)*(2^(3n+1)-7*(-1)^n+13)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
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

    /* 8^(n-1) = 2^(3(n-1))                                                     */
    seq_t pow_outer = pow2(3 * (n - 1));

    /* 2^(3n+1)                                                                */
    seq_t pow_inner = pow2(3 * n + 1);

    /* Add the constant term:  -7*(-1)^n + 13
       For n even: -7 + 13 = 6  →  pow_inner + 6
       For n odd :  7 + 13 = 20 →  pow_inner + 20                         */
    if ((n & 1) == 0)
        pow_inner += (seq_t)6;
    else
        pow_inner += (seq_t)20;

    return pow_outer * pow_inner;
}
