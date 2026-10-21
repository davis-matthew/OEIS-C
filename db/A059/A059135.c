/*
 * A059135  A hierarchical sequence (S(W2{3}c) - see A059126).
 * https://oeis.org/A059135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 64*2^n - 21*n - 53
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence satisfies a(0)=11, a(1)=54, a(2)=161 and a(n)=4*a(n-1)-5*a(n-2)+2*a(n-3) for n>2. */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^(n+6) and adjust */
    seq_t t = pow2(n + 6);
    t -= (seq_t)(21 * n);
    t -= (seq_t)53;
    return t;
}
