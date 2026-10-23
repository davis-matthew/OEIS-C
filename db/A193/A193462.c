/*
 * A193462  Sum of the distinct prime divisors of n^2+1.
 * https://oeis.org/A193462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute a(n) = sum of the distinct prime divisors of n^2+1.
    Uses the provided factor_u64 routine to factor n^2+1 efficiently. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Compute m = n^2 + 1  (fits in 64 bits for n up to 10^9) */
    uint64_t m = (uint64_t)n;
    m = m * m + 1;

    /* Factor m.  We expect very few prime factors, so a small static array suffices. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(m, primes, exps, &cnt);

    /* Sum distinct primes */
    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        sum += (seq_t)primes[i];
    }
    return sum;
}
