/*
 * A014483  Expansion of (1+2*x) / (1-2*x)^4.
 * https://oeis.org/A014483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(n-1) * (6 + 13*n + 9*n^2 + 2*n^3)) / 3 , a(0)=1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 20, u64 n <= 48, u128 n <= 109,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;          /* n^2 */
    seq_t N3 = N2 * N;         /* n^3 */

    /* P = 6 + 13*n + 9*n^2 + 2*n^3   (P is always divisible by 3) */
    seq_t P = (seq_t)6 + (seq_t)13 * N + (seq_t)9 * N2 + (seq_t)2 * N3;

    seq_t Q = P / (seq_t)3;    /* P/3, integer division */

    seq_t pow = pow2(n - 1);   /* 2^(n-1) */

    return pow * Q;
}
