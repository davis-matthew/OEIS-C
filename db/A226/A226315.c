/*
 * A226315  a(n) = (n^2/8+3*n/8-2)*2^n + 3.
 * https://oeis.org/A226315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2+3*n-16)*2^(n-3) + 3, with a(1)=a(2)=0
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 55, u128 n <= 117,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the offseted initial values */
    if (n < 3) return (seq_t)0;

    /* compute (n^2 + 3*n - 16) */
    seq_t nn = (seq_t)n * (seq_t)n;
    seq_t term = nn + (seq_t)3 * n;
    term -= (seq_t)16;

    /* compute 2^(n-3) with the utility routine */
    seq_t pw = pow2(n - 3);

    /* multiply and add the constant 3 */
    seq_t res = term * pw;
    res += (seq_t)3;
    return res;
}
