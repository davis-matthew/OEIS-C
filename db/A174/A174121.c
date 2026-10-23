/*
 * A174121  Partial sums of A001580.
 * https://oeis.org/A174121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n-2)*(2*n*n + n + 3))/6 + 2^n (offset 1)
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    /* For n ≥ 2 the first factor (n-2) is non‑negative.
       Compute the polynomial part exactly, then add 2^n. */
    seq_t nn     = (seq_t)n;          /* cast n to seq_t for arithmetic   */
    seq_t term   = (seq_t)(n - 2);    /* (n-2) part                         */
    seq_t n2     = nn * nn;           /* n^2                                */
    seq_t t      = 2 * n2;            /* 2*n^2                              */
    t += nn;                         /* 2*n^2 + n                         */
    t += 3;                          /* 2*n^2 + n + 3                     */

    term = term * t;                 /* (n-2)*(2*n^2 + n + 3)              */
    term /= 6;                       /* divide by 6 (exact division)       */

    return term + pow2(n);           /* add 2^n */
}
