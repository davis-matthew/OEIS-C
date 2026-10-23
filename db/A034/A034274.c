/*
 * A034274  a(n)=f(n,n-1) where f is given in A034261.
 * https://oeis.org/A034274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 1)/2 * Catalan(n)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;             /* offset 1: a(1) is the first term */
    seq_t cat = catalan(n);
    seq_t nsq = (seq_t)n * (seq_t)n; /* n^2 in seq_t */
    seq_t prod = cat * (nsq + (seq_t)1); /* (n^2 + 1) * Catalan(n) */
    return prod / (seq_t)2;             /* integer division by 2 */
}
