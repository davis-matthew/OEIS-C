/*
 * A140580  a(n) = (n^2)/A048671(n), = n*A014963(n) = A140579 * [1, 2, 3,
 *          ...].
 * https://oeis.org/A140580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = n if n has more than one distinct prime factor; 
 * otherwise a(n) = n*p where p is the unique prime factor of n 
 * (i.e., when n = p^k).  For n=1, a(1)=1.  This implements 
 * a(n) = n * A014963(n) as described in the OEIS entry. 
 */

#include <stdint.h>   /* for standard integer types, if needed */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)n;

    idx_t original = n;
    idx_t p = 0;

    /* Find the smallest prime factor of n */
    for (idx_t d = 2; d <= original / d; ++d)
    {
        if (original % d == 0)
        {
            p = d;
            break;
        }
    }

    /* n is prime */
    if (p == 0)
        return (seq_t)original * (seq_t)original;

    /* Check if n is a power of the found prime factor p */
    idx_t t = original;
    while (t % p == 0)
        t /= p;

    if (t == 1)    /* n = p^k */
        return (seq_t)original * (seq_t)p;

    /* n has more than one distinct prime factor */
    return (seq_t)original;
}
