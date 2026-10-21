/*
 * A088578  a(n) = n*x^n + (n-1)*x^(n-1) + . . . + x + 1 for x=2.
 * https://oeis.org/A088578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*2^(n+1)+3 (OEIS A088578)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 26, u64 n <= 57, u128 n <= 120,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */
    seq_t nm1 = (seq_t)(n - 1);   /* (n-1) as seq_t */
    seq_t pow = pow2(n + 1);     /* 2^(n+1) as seq_t */
    return nm1 * pow + (seq_t)3; /* (n-1)*2^(n+1) + 3 */
}
