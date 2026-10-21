/*
 * A073395  Product of sums of prime factors of n: with and without
 *          repetition.
 * https://oeis.org/A073395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = A008472(n) * A001414(n) where A008472(n) is the sum of
 *            prime factors of n counting multiplicity and A001414(n) is the
 *            sum of distinct prime factors of n. a(1) = 0
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition */
    if (n <= 1)
        return (seq_t)0;

    /* factor_u64 is a static helper that writes the
       prime factors and their exponents in the provided
       arrays.  We limit to 64 factors, which is more
       than enough for 64‑bit n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum_distinct = 0;   /* sum of distinct primes */
    seq_t sum_mult = 0;       /* sum of primes with multiplicity */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t e = (seq_t)exps[i];
        sum_distinct += p;
        sum_mult += p * e;
    }

    return sum_mult * sum_distinct;
}
