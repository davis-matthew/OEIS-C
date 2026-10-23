/*
 * A145399  Dirichlet g.f.: (1+4/2^s+1/4^s)*zeta(s)^3.
 * https://oeis.org/A145399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 9915, u32 n <= 9915, u64 n <= 9915,
 *            u128 n <= 9915, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A145399
   Multiplicative with a(2^e) = 3*e*(e+1)+1 and a(p^e) = (e+1)*(e+2)/2 for odd primes p.
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    uint64_t x = (uint64_t)n;         /* work in native 64‑bit integers for factoring   */
    seq_t prod = 1;                   /* product of the contributions of all prime powers */

    /* Handle factor 2 first */
    unsigned e = 0;
    while ((x & 1ULL) == 0) {
        x >>= 1;
        ++e;
    }
    if (e) {
        /* a(2^e) = 3*e*(e+1)+1 */
        unsigned long long tmp = 3ULL * e * (e + 1ULL) + 1ULL;
        prod = prod * (seq_t)tmp;
    }

    /* Trial division for odd primes up to sqrt(x) */
    for (uint64_t p = 3; p <= x / p; p += 2) {
        if (x % p == 0) {
            e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            /* a(p^e) = (e+1)*(e+2)/2 */
            unsigned long long tmp = (unsigned long long)(e + 1) * (e + 2) / 2;
            prod = prod * (seq_t)tmp;
        }
    }

    /* Remaining factor, if any, is a prime greater than sqrt(original n) */
    if (x > 1) {
        /* exponent e = 1 */
        unsigned long long tmp = (unsigned long long)(1 + 1) * (1 + 2) / 2; /* (2)*(3)/2 = 3 */
        prod = prod * (seq_t)tmp;
    }

    return prod;
}
