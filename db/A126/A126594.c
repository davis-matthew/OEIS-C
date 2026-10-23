/*
 * A126594  Floor of the average of the prime factors of n with multiplicity.
 * https://oeis.org/A126594
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(A001414(n)/A001222(n))
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;   /* undefined for n < 2 (offset 2) */

    uint64_t x = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;

    factor_u64(x, primes, exps, &cnt);

    seq_t sum   = 0;
    seq_t omega = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        sum   += (seq_t)primes[i] * (seq_t)exps[i];
        omega += (seq_t)exps[i];
    }

    return sum / omega;   /* integer division is floor for positive numbers */
}
