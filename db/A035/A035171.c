/*
 * A035171  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s) + Kronecker(m,p)*p^(-2s))^(-1) for m
 *          = -19.
 * https://oeis.org/A035171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   * a(n) is multiplicative. * a(1) = 1. * a(19^e) = 1 for every e
 *            ≥ 0. * For p = 2 or any odd prime p ≠ 19: * If Kronecker(-19, p)
 *            = 1 → a(p^e) = e + 1. * If Kronecker(-19, p) = -1 → a(p^e) = (1
 *            + (-1)^e) / 2. * * Kronecker(-19, p) equals the Legendre symbol
 *            (p|19) except for p = 2, * where it equa...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   kronecker_m19
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/kronecker_m19.h"

/* multiplicative factor for a prime power p^e */
static seq_t
prime_power_factor(uint64_t p, uint64_t e)
{
    if (p == 19) return (seq_t)1;   /* a(19^e) = 1 */

    int symbol = kronecker_m19(p);
    if (symbol == 1) {
        /* split prime */
        return (seq_t)(e + 1);
    } else { /* symbol == -1 */
        return (seq_t)((e & 1ULL) == 0 ? 1 : 0);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined below the offset */

    seq_t result = 1;
    uint64_t tmp   = (uint64_t)n;
    uint64_t e, p;

    /* factor 2 separately */
    e = 0;
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;
        ++e;
    }
    if (e > 0) {
        result *= prime_power_factor(2ULL, e);
        if (result == 0) return result;
    }

    /* odd prime factors */
    for (p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            result *= prime_power_factor(p, e);
            if (result == 0) return result;
        }
    }

    /* remaining prime factor > sqrt(tmp) */
    if (tmp > 1) {
        result *= prime_power_factor(tmp, 1);
    }

    return result;
}
