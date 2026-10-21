/*
 * A053585  If n = p_1^e_1 * ... * p_k^e_k, p_1 < ... < p_k primes, then a(n)
 *          = p_k^e_k.
 * https://oeis.org/A053585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = largest prime power dividing n (i.e. the highest power of
 *            the largest prime factor of n)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    idx_t m = n;          /* copy of n for factoring          */
    idx_t max_p = 0;      /* largest prime found              */
    idx_t max_e = 0;      /* exponent of that prime           */

    /* factor 2 separately */
    if (m % 2 == 0) {
        idx_t e = 0;
        while (m % 2 == 0) {
            m /= 2;
            ++e;
        }
        max_p = 2;
        max_e = e;
    }

    /* trial divide by odd candidates up to sqrt(m) */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            max_p = p;
            max_e = e;
        }
    }

    /* if anything remains, it is a prime factor > 2 */
    if (m > 1) {
        max_p = m;
        max_e = 1;
    }

    /* compute p^e */
    seq_t result = 1;
    for (idx_t i = 0; i < max_e; ++i)
        result *= (seq_t)max_p;

    return result;
}
