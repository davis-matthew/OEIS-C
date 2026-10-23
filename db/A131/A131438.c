/*
 * A131438  a(n) = (2+n)*2^n-2-3*n.
 * https://oeis.org/A131438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+2)*2^n - 2 - 3*n
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Convert the index to the result type to avoid premature truncation */
    seq_t nn   = (seq_t)n;
    seq_t p2   = pow2(n);           /* 2^n */
    seq_t res  = (nn + (seq_t)2) * p2;  /* (n+2)*2^n */
    res -= (seq_t)2;                 /* subtract 2 */
    res -= (seq_t)3 * nn;            /* subtract 3*n */
    return res;
}
