/*
 * A140647  Number of car parking assignments of n cars in n spaces, if one
 *          car does not park.
 * https://oeis.org/A140647
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(n+2)^(n-1) - (3*n+1)*(n+1)^(n-2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    /* convert n to seq_t only for the final arithmetic */
    seq_t N = (seq_t)n;

    /* compute (n+2)^(n-1) and (n+1)^(n-2) using the provided power helper */
    seq_t pow1 = pow_uint((uint64_t)(N + 2), n - 1);   /* (n+2)^(n-1)   */
    seq_t pow2 = pow_uint((uint64_t)(N + 1), n - 2);   /* (n+1)^(n-2)   */

    /* construct the two terms of the formula */
    seq_t term1 = (seq_t)2 * pow1;                     /* 2*(n+2)^(n-1) */
    seq_t term2 = (seq_t)(3 * n + 1) * pow2;           /* (3*n+1)*(n+1)^(n-2) */

    return term1 - term2;                              /* a(n)          */
}
