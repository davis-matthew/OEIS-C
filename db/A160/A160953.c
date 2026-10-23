/*
 * A160953  a(n) = Sum_{d|n} Moebius(n/d)*d^(b-1)/phi(n) for b = 10.
 * https://oeis.org/A160953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} Moebius(n/d)*d^(9)/phi(n) (here b = 10)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 14, u64 n <= 227, u128 n <= 5000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalently:  a(n) = J_9(n)/phi(n) = product_{p^e||n}  p^(8e-8) * (p^9-1)/(p-1) */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used, but safe */
    unsigned __int128 result = 1;

    uint64_t m = (uint64_t)n;

    /* trial division factorisation (fast enough for n <= 10000) */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                ++e;
                m /= p;
            }

            /* compute p^(8e-8) */
            unsigned __int128 pow8e = 1;
            for (uint64_t k = 0; k < 8 * e - 8; ++k)
                pow8e *= (unsigned __int128)p;

            /* compute (p^9 - 1) / (p - 1) */
            unsigned __int128 p9 = 1;
            for (uint64_t k = 0; k < 9; ++k)
                p9 *= (unsigned __int128)p;
            unsigned __int128 factor = (p9 - 1) / (p - 1);

            /* multiply the contribution of this prime power */
            result *= pow8e * factor;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        uint64_t p = m;          /* exponent e = 1 */
        /* p^(8*1-8) = p^0 = 1 */
        unsigned __int128 pow8e = 1;

        unsigned __int128 p9 = 1;
        for (uint64_t k = 0; k < 9; ++k)
            p9 *= (unsigned __int128)p;
        unsigned __int128 factor = (p9 - 1) / (p - 1);

        result *= pow8e * factor;
    }

    return (seq_t)result;
}
