/*
 * A020539  a(n) = 5th Chebyshev polynomial (first kind) evaluated at 2^n.
 * https://oeis.org/A020539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * (5 - 5*4^(1+n) + 16^(1+n))
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           = pow2(n) * (pow2(4*n+4) + 5 - 5*pow2(2*n+2)) */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute the powers of two needed by the formula. */
    seq_t p2   = pow2(n);          /* 2^n                      */
    seq_t p4   = pow2(2*n + 2);    /* 4^(n+1)  = 2^(2n+2)      */
    seq_t p16  = pow2(4*n + 4);    /* 16^(n+1) = 2^(4n+4)      */

    /* Combine to obtain the inner bracket: 5 - 5*4^(1+n) + 16^(1+n)   */
    seq_t inner = p16;             /* start with 16^(n+1)       */
    inner += (seq_t)5;             /* +5                        */
    inner -= p4 * (seq_t)5;        /* -5*4^(n+1)                */

    /* Final result: 2^n * inner */
    return p2 * inner;
}
