/*
 * A078553  Largest prime dividing sigma(4,n).
 * https://oeis.org/A078553
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A006530(sigma_4(n)) (largest prime divisor of σ₄(n),
 *            where σ₄(n)=∑_{d|n} d⁴)
 * Range:     u8 n <= 4, u16 n <= 22, u32 n <= 312, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/largest_prime_divisor.h"

/* power of a 64‑bit base to an unsigned power, result in unsigned __int128 */
static unsigned __int128 pow128(uint64_t base, unsigned int exp)
{
    unsigned __int128 res = 1;
    for (unsigned int i = 0; i < exp; ++i)
        res *= base;
    return res;
}

/* compute σ₄(n) using the multiplicative formula */
static uint64_t sigma4_uint64(uint64_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    unsigned __int128 sigma = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        unsigned int e = (unsigned int)exps[i];

        unsigned int total = 4u * (e + 1u);
        unsigned __int128 num   = pow128(p, total) - 1;
        unsigned __int128 denom = pow128(p, 4) - 1;
        unsigned __int128 term  = num / denom;

        sigma *= term;
    }

    return (uint64_t)sigma;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                /* below the offset – behaviour irrelevant */

    uint64_t sigma4 = sigma4_uint64((uint64_t)n);
    uint64_t gpf = largest_prime_divisor(sigma4);
    return (seq_t)gpf;
}
