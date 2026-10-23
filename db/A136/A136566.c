/*
 * A136566  a(n) = sum of the exponents occurring only once each in the
 *          prime-factorization of n.
 * https://oeis.org/A136566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of exponents that occur exactly once in the prime
 *            factorization of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1; factorize using the helper */
    uint64_t primes[65];
    uint64_t exps[65];
    size_t cnt = 0;

    /* factor_u64 fills primes[], exps[] and sets cnt */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* maximum exponent for a uint64_t factorization is <= 64 */
    unsigned freq[65] = {0};

    for (size_t i = 0; i < cnt; ++i)
        if (exps[i] <= 64)
            freq[(unsigned)exps[i]]++;

    seq_t result = 0;
    for (unsigned e = 1; e <= 64; ++e)
        if (freq[e] == 1)
            result += (seq_t)e;

    return result;
}
