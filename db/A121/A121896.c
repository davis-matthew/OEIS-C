/*
 * A121896  Let M be the matrix defined in A111490. Sequence gives M(1,1),
 *          M(1,2)+M(1,2)+M(2,2), M(1,3)+M(2,3)+M(3,1)+M(3,2)+M(3,3), etc.
 * https://oeis.org/A121896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+1) = a(n) + 2*n + 1 - sum_proper(n), a(1) = 1 * where
 *            sum_proper(n) is the sum of the proper divisors of n (i.e. all *
 *            positive divisors < n). This implements the required sequence *
 *            A121896. No external libraries are needed and the function *
 *            works for any n ≥ 1 in any order of calls. The im...
 * Range:     u8 n <= 19, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute the sum of proper divisors of m.  The result is returned
 * as a seq_t; only integer addition and subtraction are used. */
static seq_t sum_of_proper_divisors(idx_t m)
{
    if (m <= 1) return (seq_t)0;
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= m; ++d)
    {
        if (m % d == 0)
        {
            /* d is a divisor.  Exclude m itself. */
            if (d != m) sum += (seq_t)d;
            idx_t e = m / d;                /* the paired divisor */
            if (e != d && e != m) sum += (seq_t)e;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1, return 0 */

    seq_t a_val = (seq_t)1;
    for (idx_t k = 1; k < n; ++k)
    {
        seq_t delta = (seq_t)(2 * k + 1) - sum_of_proper_divisors(k);
        a_val += delta;
    }
    return a_val;
}
