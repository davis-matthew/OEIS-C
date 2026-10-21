/*
 * A071930  Number of words of length 2n in the two letters s and t that
 *          reduce to the identity 1 by using the relations ssTT=1, ststSS=1
 *          and ststTT=1, where S and T are the inverses of s and t,
 *          respectively (i.e., sS=1 and tT=1). The generators s and t and the
 *          three stated relations generate the quaternion...
 * https://oeis.org/A071930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(n-1) - (-2)^(n-1) = 2^(2n-2) - (-2)^(n-1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* For n <= 0 (outside the offset) we return 0, although
       the sequence is defined for n >= 1 only. */
    if (n == 0)
        return (seq_t)0;

    /* Compute 2^(2*(n-1)) = 4^(n-1) */
    idx_t exp4 = (n > 0) ? 2 * (n - 1) : 0;
    seq_t pow4 = pow2(exp4);

    /* Compute 2^(n-1) */
    idx_t exp2 = (n > 0) ? n - 1 : 0;
    seq_t pow2n = pow2(exp2);

    /* (-2)^(n-1) = (-1)^(n-1) * 2^(n-1) */
    /* If (n-1) is even, (-1)^(n-1) = +1; subtract it.
       If (n-1) is odd, (-1)^(n-1) = -1; subtracting a negative
       is the same as adding. */
    if (((n - 1) & 1) == 0)
        return pow4 - pow2n;   /* even exponent: 4^(n-1) - 2^(n-1) */
    else
        return pow4 + pow2n;   /* odd exponent : 4^(n-1) + 2^(n-1) */
}
