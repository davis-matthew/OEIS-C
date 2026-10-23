/*
 * A187767  Number of bicolored cyclic patterns n X n.
 * https://oeis.org/A187767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1)-1 if n is odd, 2^(n-1)+a(n/2) if n is even
 *            (conjectured)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 0 */
        return (seq_t)0;

    seq_t res = (seq_t)0;
    idx_t temp = n;

    while (1)
    {
        if (temp == 1)          /* reached the base case */
            break;

        if (temp & 1ULL)        /* odd n */
        {
            seq_t term = pow2(temp - 1);
            term -= (seq_t)1;   /* 2^(n-1)-1 */
            res += term;
            break;
        }
        else                    /* even n */
        {
            seq_t term = pow2(temp - 1);
            res += term;        /* 2^(n-1)+a(n/2) => add 2^(n-1) now */
            temp /= 2;          /* recurse on n/2 */
        }
    }
    return res;
}
