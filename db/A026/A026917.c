/*
 * A026917  a(n) = Sum{T(i,j)}, 0<=j<=i, 0<=i<=n, T given by A026907.
 * https://oeis.org/A026917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 98*2^n - 12*n^2 - 60*n - 97
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 98*2^n */
    seq_t two_n  = pow2(n);              /* 2^n */
    seq_t term1  = two_n * (seq_t)98;    /* 98*2^n */

    /* Compute 12*n^2 + 60*n + 97 */
    seq_t nsq = (seq_t)n * (seq_t)n;     /* n^2 */
    seq_t sub = (seq_t)12 * nsq;         /* 12*n^2 */
    sub += (seq_t)60 * (seq_t)n;         /* +60*n */
    sub += (seq_t)97;                    /* +97 */

    /* Final result */
    return term1 - sub;
}
