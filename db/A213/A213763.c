/*
 * A213763  Principal diagonal of the convolution array A213762.
 * https://oeis.org/A213763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n*(2*n+1) - 4*n - 1. Hence a(n) = -1 + 2^n - 4*n +
 *            n*2^(n+1)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 26, u64 n <= 57, u128 n <= 120,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* For n = 0 the sequence is undefined, but returning 0 avoids UB. */
    if (n == 0) return 0;

    /* 2*n fits safely in idx_t for all practical n here (n <= 10000). */
    seq_t two_n   = (seq_t)((idx_t)2 * n);   /* 2*n as seq_t */
    seq_t pow2_n  = pow2(n);                /* 2^n as seq_t */
    seq_t ans     = pow2_n * (two_n + (seq_t)1); /* 2^n*(2*n+1) */

    ans -= (seq_t)4 * n;   /* subtract 4*n */
    ans -= 1;              /* subtract 1 */

    return ans;
}
