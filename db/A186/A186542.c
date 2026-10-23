/*
 * A186542  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f(i)=i^2 and g(j)=-2+3j^2. Complement
 *          of A186541.
 * https://oeis.org/A186542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(sqrt(3n^2 - 5/2))
 * Range:     u8 n <= 4, u16 n <= 27, u32 n <= 3513, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    /* compute 3n^2 - 5/2, but use integer arithmetic.
       3n^2 - 5/2 = (6n^2 - 5)/2.  The floor of the square root
       of this expression can be obtained by taking the integer
       floor of the numerator, dividing by 2, and then taking the
       integer square root.  This is exact for all integer n.          */
    seq_t num = (seq_t)6 * (seq_t)n * (seq_t)n - 5;   /* 6n^2 - 5 */
    seq_t half = num / 2;                            /* floor((6n^2-5)/2) */
    seq_t k = isqrt_seq(half);                       /* floor sqrt(half) */
    return (seq_t)n + k;
}
