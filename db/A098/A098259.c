/*
 * A098259  First differences of Chebyshev polynomials S(n,531) = A098257(n)
 *          with Diophantine property.
 * https://oeis.org/A098259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   OEIS A098259 – first differences of Chebyshev polynomials S(n,531)
   Recurrence:
     a(0) = 1
     a(1) = 530
     a(n) = 531*a(n-1) - a(n-2)   for n ≥ 2
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)530;

    seq_t prevprev = (seq_t)1;  /* a(0) */
    seq_t prev     = (seq_t)530;/* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = (seq_t)531 * prev - prevprev;
        prevprev = prev;
        prev     = cur;
    }
    return prev;
}
