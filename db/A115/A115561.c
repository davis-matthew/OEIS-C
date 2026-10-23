/*
 * A115561  a(n) = lpf((n/lpf(n))/lpf(n/lpf(n))), where lpf=A020639, least
 *          prime factor.
 * https://oeis.org/A115561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = lpf((n/lpf(n))/lpf(n/lpf(n)))
 * Range:     u8 n <= 1027, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/lpf.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;

    /* First least prime factor of n */
    idx_t p1 = lpf(n);
    if (p1 == 0 || p1 == n)
        return 1;   /* n is 1 or prime */

    /* Divide n by its first prime factor */
    idx_t t1 = n / p1;

    /* Second least prime factor of the reduced number */
    idx_t p2 = lpf(t1);
    if (p2 == 0 || p2 == t1)
        return 1;   /* only two prime factors in total */

    /* Remaining factor after removing the first two */
    idx_t t2 = t1 / p2;
    if (t2 <= 1)
        return 1;   /* no third factor */

    /* Least prime factor of the remaining part is the answer */
    idx_t p3 = lpf(t2);
    if (p3 <= 1)
        return 1;

    return (seq_t)p3;
}
