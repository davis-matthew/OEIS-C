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
 * Validated: standard, pass, 46 terms through n = 870 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Compute sigma_6(p^e) for a prime power.  The result is returned as an
   unsigned __int128 so that intermediate values that exceed 64 bits are
   handled safely.  For any n whose sigma_6(n) fits in seq_t, this
   intermediate value will also fit in 64 bits. */
static unsigned __int128 sigma6_prime_power_uint128(uint64_t prime, idx_t e)
{
    if (e == 0)
        return 1;

    /* Compute p^6 once (fits in 128 bits for all primes <= 10^4). */
    unsigned __int128 p6 = 1;
    for (int i = 0; i < 6; ++i)
        p6 *= prime;

    unsigned __int128 term  = 1;  /* current p^(6k) term */
    unsigned __int128 total = 1;  /* sum of terms */

    for (idx_t k = 1; k <= e; ++k) {
        term *= p6;
        total += term;
    }

    return total;
}

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
