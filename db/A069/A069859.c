/*
 * A069859  a(n) = (greatest prime factor of n) modulo (least prime factor of
 *          n).
 * https://oeis.org/A069859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (greatest prime factor of n) modulo (least prime factor
 *            of n)
 * Range:     u8 n <= 9552, u16 n <= 9552, u32 n <= 9552, u64 n <= 9552,
 *            u128 n <= 9552, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)               /* a(1) = 0 by definition */
        return (seq_t)0;

    uint64_t m = (uint64_t)n;
    uint64_t lpf = 0;         /* least prime factor */
    uint64_t gpf = 0;         /* greatest prime factor */

    /* trial division up to sqrt(m) */
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            if (lpf == 0)      /* first factor encountered */
                lpf = p;
            gpf = p;          /* current prime is the largest seen so far */

            /* remove all occurrences of p from m */
            while (m % p == 0)
                m /= p;
        }
    }

    /* if m > 1, then m itself is a prime factor larger than any p examined */
    if (m > 1)
    {
        if (lpf == 0)          /* n itself was prime */
            lpf = m;
        gpf = m;
    }

    /* for prime n, lpf = gpf = n, hence result is 0 */
    return (seq_t)(gpf % lpf);
}
