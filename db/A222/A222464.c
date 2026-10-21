/*
 * A222464  a(n) = (n+6)/gcd(n*6,n+6), n >= 6.
 * https://oeis.org/A222464
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+6)/gcd(n*6,n+6) = (n+6)/gcd(36,n+6) for n >= 6
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 6)
        return 0;
    seq_t sum = (seq_t)n + (seq_t)6;        /* n+6 */
    seq_t g   = gcd(sum, (seq_t)36);        /* gcd(36, n+6) */
    return sum / g;                         /* (n+6)/gcd(36,n+6) */
}
