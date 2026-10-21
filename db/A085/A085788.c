/*
 * A085788  Partial sums of n 3-spaced triangular numbers beginning with t(3),
 *          e.g., a(2) = t(3)+t(6) = 6+21 = 27.
 * https://oeis.org/A085788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3/2) * n * (n + 1)^2 (also a(n) = 3 * A006002(n) where
 *            A006002(n) = n*(n+1)^2/2)
 * Range:     u8 n <= 4, u16 n <= 34, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t */

seq_t a(idx_t n)
{
    /* Convert index to seq_t early to avoid mixing types */
    seq_t nseq = (seq_t)n;
    seq_t n1   = nseq + 1;          /* (n + 1) */
    seq_t t    = nseq * n1 / 2;     /* triangular number n*(n+1)/2 */
    /* a(n) = 3 * t * (n + 1) */
    return t * n1 * 3;
}
