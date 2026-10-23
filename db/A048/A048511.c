/*
 * A048511  a(n) = T(6,n), array T given by A048505.
 * https://oeis.org/A048511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+9)*(n+16) * 2^(n-2) - 35
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 23, u64 n <= 53, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the two special cases that would involve a negative
       exponent for 2.  For n = 0 and n = 1 the formula reduces to
       integer values that can be hard‑coded. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)50;

    /* For all n ≥ 2 the expression can be evaluated safely using
       unsigned arithmetic; the shift amount (n-2) is non‑negative. */
    seq_t term = (seq_t)(n + 9) * (seq_t)(n + 16);
    term <<= (n - 2);        /* multiply by 2^(n-2) */
    term -= (seq_t)35;
    return term;
}
