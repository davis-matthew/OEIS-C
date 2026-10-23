/*
 * A191007  a(n) = n*2^(n+1) + (2^(n+3)+(-1)^n)/3.
 * https://oeis.org/A191007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = n*2^(n+1) + (2^(n+3)+(-1)^n)/3
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 26, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t pow_n1  = pow2(n + 1);                     /* 2^(n+1)          */
    seq_t term1   = ((seq_t)n) * pow_n1;             /* n*2^(n+1)         */

    seq_t pow_n3  = pow2(n + 3);                     /* 2^(n+3)           */
    seq_t numerator;
    if ((n & 1) == 0) {                              /* even n            */
        numerator = pow_n3 + (seq_t)1;               /* 2^(n+3)+1         */
    } else {                                         /* odd n             */
        numerator = pow_n3 - (seq_t)1;               /* 2^(n+3)-1         */
    }
    seq_t term2 = numerator / (seq_t)3;              /* (2^(n+3)+(-1)^n)/3*/

    return term1 + term2;                            /* sum of the two terms */
}
