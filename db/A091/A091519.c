/*
 * A091519  G.f.: Sum_{k>=0} (2^k*t*(1+t)/(1-t)^3, t=x^2^k).
 * https://oeis.org/A091519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(0)=0; a(2n)=2*a(n)+(2n)^2; a(2n+1)=(2n+1)^2
 * Range:     u8 n <= 13, u16 n <= 191, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (i.e., a(n)=n^2 when n is odd, and a(2n)=2*a(n)+4n^2 otherwise) */

#include <stddef.h>

seq_t a(idx_t n)
{
    // Base case
    if (n == 0) {
        return 0;
    }

    // If n is odd, a(n) = n^2
    if (n & 1) {
        return (seq_t)n * (seq_t)n;
    }

    // If n is even, compute recursively
    seq_t half = a(n >> 1);                 /* a(n/2) */
    seq_t n_sq = (seq_t)n * (seq_t)n;       /* (2n)^2 = n^2 */
    return half + half + n_sq;              /* 2*a(n/2) + n^2 */
}
