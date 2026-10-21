/*
 * A214587  Greatest common divisor of a number and its last decimal digit:
 *          a(n) = gcd(n, n mod 10).
 * https://oeis.org/A214587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = gcd(n, n mod 10)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* last decimal digit of n */
    seq_t last = (seq_t)(n % 10u);

    /* If the last digit is 0, a(n) = n.  The cast will wrap for
       seq_t types that cannot hold the full value – tests only
       compare results that fit in seq_t. */
    if (last == 0)
        return (seq_t)n;

    /* gcd(n, last) = gcd(last, n mod last).  Both operands now
       fit in seq_t. */
    seq_t rem = (seq_t)(n % (idx_t)last);
    return seq_gcd(last, rem);
}
