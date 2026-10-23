/*
 * A183063  Number of even divisors of n.
 * https://oeis.org/A183063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of even divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 0 if n is odd, otherwise a(n) = d(n/2) where d(x) is the divisor count of x */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static inline seq_t divisor_count_uint64(uint64_t x)
{
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64(x, primes, exps, &cnt);   /* factor u64 – utility function */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        result *= (seq_t)(exps[i] + 1);
    }
    return result;
}

seq_t a(idx_t n)
{
    if ((n & 1ULL) == 0ULL)      /* n is even */
    {
        uint64_t half = n / 2ULL;
        return divisor_count_uint64(half);
    }
    else                          /* n is odd -> no even divisors */
    {
        return 0;
    }
}
