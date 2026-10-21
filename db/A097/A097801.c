/*
 * A097801  a(n) = (2*n)!/(n!*2^(n-1)).
 * https://oeis.org/A097801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * (2*n-1)!! Recurrence: a(0) = 2, a(n) = a(n-1) *
 *            (2*n-1) (n >= 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, but seq_t/idx_t are supplied by the build */

seq_t a(idx_t n)
{
    /* a(0) is 2, and for n>0 the recurrence multiplies by each odd
       integer up to (2*n-1).  This implements a(n)=2*(2*n-1)!! directly. */
    seq_t res = (seq_t)2;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        res *= (seq_t)(2 * i - 1);
    }
    return res;
}
