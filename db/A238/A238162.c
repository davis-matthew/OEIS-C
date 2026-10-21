/*
 * A238162  Least common multiple of the prime factors of n, each increased by
 *          1.
 * https://oeis.org/A238162
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm{ p + 1 | p is a prime divisor of n } (offset 2)
 * Range:     u8 n <= 193, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* a(n) is only defined for n ≥ 2; for lower indices return 0 */
    if (n < 2)
        return 0;

    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    /* factor_u64: factors n into primes[] and exps[], count in cnt */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t res = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p1 = (seq_t)(primes[i] + 1);
        seq_t g  = gcd(res, p1);
        res = (res / g) * p1;
    }
    return res;
}
