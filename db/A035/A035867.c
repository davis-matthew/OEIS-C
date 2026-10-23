/*
 * A035867  Coordination sequence for A_41 lattice.
 * https://oeis.org/A035867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(n) = sum_{d=1..41} C(42,d) * C(n-1,d-1) * C(n+41-d , n)
 *   (where C(a,b) = binomial coefficient, taken as 0 if b>a)
 */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* The coordination sequence is defined for n>=0, but the above
       closed form only works for n>0.  The a(0) entry is 1. */
    if (n == 0)
        return (seq_t)1;

    /* For d-1 > n-1 the inner binomial is zero, so we can restrict
       the loop to d <= n.  The outer bound is 41. */
    idx_t max_d = 41;
    if (max_d > n)
        max_d = n;

    seq_t sum = 0;

    for (idx_t d = 1; d <= max_d; ++d)
    {
        seq_t term = binomial(42, d);
        term *= binomial(n - 1, d - 1);
        term *= binomial(n + 41 - d, n);
        sum += term;
    }

    return sum;
}
