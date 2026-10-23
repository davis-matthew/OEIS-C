/*
 * A101605  a(n) = 1 if n is a product of exactly 3 (not necessarily distinct)
 *          primes, otherwise 0.
 * https://oeis.org/A101605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 if n has exactly three prime factors (counting
 *            multiplicity), otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build system; no need to typedef them here. */

seq_t a(idx_t n)
{
    /* Count prime factors of n (with multiplicity). */
    int cnt = 0;
    idx_t m = n;           /* make a copy to preserve input */
    for (idx_t p = 2; p <= m / p; ++p)   /* p <= sqrt(m) */
    {
        if (m % p == 0)
        {
            while (m % p == 0)
            {
                ++cnt;
                if (cnt > 3)   /* early exit */
                    return (seq_t)0;
                m /= p;
            }
        }
    }
    if (m > 1)               /* remaining prime factor */
        ++cnt;
    return (seq_t)(cnt == 3);
}
