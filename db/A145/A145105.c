/*
 * A145105  a(n) = n if n is prime or a perfect number, otherwise a(n) = 0.
 * https://oeis.org/A145105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = n if n is prime or a perfect number, a(n) = 0 otherwise
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return true iff x is a perfect number.
   Perfect numbers are of the form 2^(p-1)*(2^p-1) where 2^p-1 is prime. */
static bool is_perfect(idx_t x)
{
    /* Perfect numbers are even. */
    if ((x & 1) != 0)
        return false;

    /* Remove all factors of two: x = 2^e * m with m odd. */
    idx_t t = x;
    idx_t e = 0;
    while ((t & 1) == 0) {
        t >>= 1;
        ++e;
    }
    idx_t m = t;          /* odd part */
    idx_t mp1 = m + 1;    /* m+1 = 2^p            */

    /* mp1 must be a power of two. */
    if (mp1 == 0 || (mp1 & (mp1 - 1)) != 0)
        return false;

    /* Also its exponent must be e+1: mp1 >> e should equal 2. */
    if ((mp1 >> e) != 2)
        return false;

    /* 2^p-1 (i.e., m) must be prime. */
    return is_prime(m);
}

seq_t a(idx_t n)
{
    if (is_prime(n) || is_perfect(n))
        return (seq_t)n;
    return 0;
}
