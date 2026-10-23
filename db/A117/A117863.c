/*
 * A117863  Number of palindromes (in base 4) below 4^n.
 * https://oeis.org/A117863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5*4^((n-1)/2)-2 for n odd, 2*4^(n/2)-2 for n even
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1, safe guard */

    /* exponent for 2^exp   exp = 2*floor(n/2) = n (even) or n-1 (odd) */
    idx_t exp = (n >> 1) << 1;             /* 2 * floor(n/2) */

    seq_t pow = pow2(exp);                /* pow = 2^exp = 4^floor(n/2) */

    if (n & 1) {                          /* odd n */
        pow = pow * (seq_t)5;             /* 5 * 4^((n-1)/2) */
        pow = pow - (seq_t)2;             /* - 2 */
    } else {                              /* even n */
        pow = pow * (seq_t)2;             /* 2 * 4^(n/2)   */
        pow = pow - (seq_t)2;             /* - 2 */
    }

    return pow;
}
