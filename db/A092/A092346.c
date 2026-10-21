/*
 * A092346  a(0)=1; a(n) = sigma_1(n) + sigma_2(n).
 * https://oeis.org/A092346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma_1(n) + sigma_2(n) (with a(0)=1)
 * Range:     u8 n <= 13, u16 n <= 209, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast integer power (unsigned __int128), base <= 10^19, exponent <= 64 */
static unsigned __int128 pow_uint128(uint64_t base, unsigned int exp)
{
    unsigned __int128 result = 1;
    unsigned __int128 b = base;
    while (exp) {
        if (exp & 1U) result *= b;
        b *= b;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    uint64_t m = (uint64_t)n;
    unsigned __int128 sigma1 = 1;
    unsigned __int128 sigma2 = 1;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            unsigned int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }

            /* sigma_1 factor */
            unsigned __int128 pe1 = pow_uint128(p, e + 1);
            sigma1 *= (pe1 - 1) / (p - 1);

            /* sigma_2 factor */
            uint64_t p2 = p * p;
            unsigned __int128 pe2 = pow_uint128(p2, e + 1);
            sigma2 *= (pe2 - 1) / (p2 - 1);
        }
    }

    if (m > 1) {
        /* Remaining prime factor m */
        uint64_t p = m;
        unsigned __int128 pe1 = pow_uint128(p, 2);          /* p^(1+1) */
        sigma1 *= (pe1 - 1) / (p - 1);

        uint64_t p2 = p * p;
        unsigned __int128 pe2 = pow_uint128(p2, 2);          /* (p^2)^(1+1) = p^4 */
        sigma2 *= (pe2 - 1) / (p2 - 1);
    }

    unsigned __int128 sum = sigma1 + sigma2;
    return (seq_t)sum;
}
