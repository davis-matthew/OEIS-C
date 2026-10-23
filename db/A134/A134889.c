/*
 * A134889  a(n)=the largest sum of two nontrivial divisors of n, if any,
 *          whose product equals n; otherwise, a(n)=n.
 * https://oeis.org/A134889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest sum d + e of two nontrivial divisors d, e of n
 *            with d*e = n; if n has no such divisor pair (i.e., n is 1 or
 *            prime), a(n) = n
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For all n >= 1.  If n is 1 or prime, the loop will not find a divisor
       and we return n itself.  For composite n, the first divisor found
       is the smallest nontrivial divisor d, and the sum d + n/d is maximal. */
    for (idx_t d = 2; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;
            return (seq_t)d + (seq_t)e;
        }
    }
    return (seq_t)n;
}
