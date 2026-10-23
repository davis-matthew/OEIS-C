/*
 * A190216  Let s(k) be the sum of the decimal digits of a number k. a(n) is
 *          the smallest k such that s(k)*(s(k)+n)=k, or 0 if no such k
 *          exists.
 * https://oeis.org/A190216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = smallest k > 0 such that s(k)*(s(k)+n)=k, where s(k) is
 *            the sum of decimal digits of k; returns 0 if no such k exists.
 *            This implementation searches over possible digit sums s (1 <= s
 *            <= 500), which is a safe upper bound for all n in the problem
 *            domain
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Iterate over all plausible digit sums s. */
    for (idx_t s = 1; s <= 500; ++s) {
        /* Work in seq_t to avoid overflow for arbitrary-precision. */
        seq_t ss = (seq_t)s;
        seq_t k  = ss * (ss + (seq_t)n);      /* k = s*(s+n)        */
        if (digit_sum(k) == ss)               /* check s(k) == s     */
            return k;                         /* found the required k */
    }
    return (seq_t)0;                         /* no solution found   */
}
