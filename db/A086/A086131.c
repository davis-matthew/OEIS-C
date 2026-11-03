/*
 * A086131  Largest prime factor of arithmetic derivative of n if it exists,
 *          or a(n)=0 for n=1 and n=prime.
 * https://oeis.org/A086131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 317, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 108 terms through n = 9248 (2026-11-01)
 * Helpers:   factor_u64, is_prime, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Largest prime factor of the arithmetic derivative of n.
   a(n) = 0 for n = 1 or n prime.  For composite n,
   a(n) = largest prime factor of n' where n' = n * sum(k_i / p_i)
   over the prime factorisation n = Π p_i^{k_i}. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n <= 1)
    {
        return (seq_t)0;
    }
    if (is_prime(n))
    {
        return (seq_t)0;
    }

    uint64_t n64 = (uint64_t)n;      /* n fits in uint64_t (idx_t) */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(n64, primes, exps, &cnt);

    uint64_t derivative = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        /* term = k_i * (n/p_i) */
        uint64_t part = n64 / primes[i];
        derivative += part * exps[i];
    }

    if (derivative <= 1)
    {
        return (seq_t)0;
    }

    uint64_t lp = largest_prime_factor(derivative);
    return (seq_t)lp;
}
