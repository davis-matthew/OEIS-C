/*
 * A073490  Number of prime gaps in factorization of n.
 * https://oeis.org/A073490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = number of gaps between successive distinct prime factors
 *            of n, where a gap occurs if the next prime after a prime factor
 *            is not the following factor
 * Range:     u8 n <= 9785, u16 n <= 9785, u32 n <= 9785, u64 n <= 9785,
 *            u128 n <= 9785, big any n
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;
    uint64_t u = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(u, primes, exps, &cnt);
    if (cnt < 2)
        return 0;
    seq_t gaps = 0;
    for (size_t i = 1; i < cnt; ++i) {
        uint64_t expected = next_prime(primes[i-1]);
        if (expected != primes[i])
            ++gaps;
    }
    return gaps;
}
