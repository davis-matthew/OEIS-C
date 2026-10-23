/*
 * A121133  Numbers of isomers of unbranched a-4-catapolypentagons - see
 *          Brunvoll reference for precise definition.
 * https://oeis.org/A121133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=1, a(3)=2, a(4)=3; for n≥5 if n even: a(n) =
 *            2^(n-5)*(n+2); if n odd : a(n) = 2^(n-5)*(n+2) + 2^((n-5)/2);
 *            (the recurrence a(n)=4*a(n-1)-2*a(n-2)-8*a(n-3)+8*a(n-4) holds
 *            for n>7)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 62, u128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 1;
    if (n == 3) return 2;
    if (n == 4) return 3;

    /* n ≥ 5 */
    idx_t exp = n - 5;                 /* exponent for the main power of two */
    seq_t pow_main = pow2(exp);        /* 2^(n-5) */
    seq_t mult = pow_main * ((seq_t)n + 2);   /* 2^(n-5) * (n+2) */

    if ((n & 1) == 0) /* even n */
        return mult;

    /* odd n: add the secondary power of two */
    seq_t pow_extra = pow2(exp / 2);   /* 2^((n-5)/2) */
    return mult + pow_extra;
}
