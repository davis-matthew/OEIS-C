/*
 * A208640  Number of 5 X n 0..1 arrays with new values 0..1 introduced in row
 *          major order and no element equal to more than one of its immediate
 *          leftward or upward or right-upward antidiagonal neighbors.
 * https://oeis.org/A208640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -84 + 63*2^n - (191*n + n^3)/6 - 4*n^2 for n > 2 a(1) =
 *            16, a(2) = 89 (Recurrence for n>7:
 *            a(n)=6a(n-1)-14a(n-2)+16a(n-3)-9a(n-4)+2a(n-5))
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)89;

    seq_t N      = (seq_t)n;
    seq_t pow2n  = pow2(n);                 /* 2^n */
    seq_t term1  = (seq_t)63 * pow2n;       /* 63*2^n */
    seq_t result = term1 - (seq_t)84;       /* term1 - 84 */

    seq_t n2 = N * N;                      /* n^2 */
    result -= (seq_t)4 * n2;               /* -4*n^2 */

    seq_t n3 = n2 * N;                     /* n^3 */
    seq_t numerator = (seq_t)191 * N + n3; /* 191*n + n^3 */
    seq_t term4 = numerator / (seq_t)6;    /* (191*n + n^3)/6 */
    result -= term4;                       /* subtract the fraction part */

    return result;
}
