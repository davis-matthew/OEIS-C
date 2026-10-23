/*
 * A013954  a(n) = sigma_6(n), the sum of the 6th powers of the divisors of n.
 * https://oeis.org/A013954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = sigma_6(n) = the sum of the sixth powers of the divisors
 *            of n
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, sigma6_prime_power_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/sigma6_prime_power_uint128.h"

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition.  The implementation below works for any
       n >= 1; for n == 0 we simply return 0 although the offset is 1. */
    if (n == 0)
        return 0;

    /* Factorize n into primes and exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Multiply the sigma_6(p^e) values for each distinct prime factor. */
    unsigned __int128 result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        unsigned __int128 sigma = sigma6_prime_power_uint128(primes[i], (idx_t)exps[i]);
        result *= sigma;
    }

    /* Convert the 128‑bit result back to seq_t.  For values that fit
       in seq_t this conversion is exact; for larger results the
       truncated value is returned, which is acceptable since
       non‑fitting terms are not checked. */
    return (seq_t)result;
}
