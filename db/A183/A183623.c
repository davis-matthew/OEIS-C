/*
 * A183623  Number of (n+1) X (n+1) 0..2 arrays with every 2 X 2 subblock
 *          summing to 4.
 * https://oeis.org/A183623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(n+1)-2)^2 + 2*3^(n+1) - 3
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t two_pow = pow2(n + 1);          /* 2^(n+1) */
    seq_t sub = two_pow - (seq_t)2;       /* 2^(n+1) - 2 */
    seq_t term1 = sub * sub;              /* (2^(n+1)-2)^2 */

    seq_t three_pow = pow3(n + 1);        /* 3^(n+1) */
    seq_t term2 = three_pow * (seq_t)2;   /* 2 * 3^(n+1) */

    seq_t res = term1 + term2;
    res = res - (seq_t)3;                 /* subtract 3 */

    return res;
}
