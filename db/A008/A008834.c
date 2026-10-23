/*
 * A008834  Largest cube dividing n.
 * https://oeis.org/A008834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = largest cube divisor of n
 * Range:     u8 n <= 342, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* It is computed by factoring n, taking the exponent of each prime
   down to the nearest multiple of three, and multiplying the
   corresponding prime powers. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* For n = 0 this definition is irrelevant; the sequence
       starts at n = 1. */
    if (n <= 1)
        return (seq_t)1;

    /* Maximum number of distinct prime factors for a 64‑bit integer
       is less than 64, so a small static buffer is sufficient. */
    uint64_t primes[64];
    uint64_t expns[64];
    size_t cnt = 0;

    /* Factor the integer n. */
    factor_u64((uint64_t)n, primes, expns, &cnt);

    /* Build the largest cube divisor. */
    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t e = expns[i];
        /* Reduce exponent to the largest multiple of 3 not larger
           than e. */
        uint64_t m = (e / 3) * 3;
        if (m == 0)
            continue;

        seq_t p = (seq_t)primes[i];

        /* Compute p^m efficiently by grouping into powers of 3. */
        seq_t pow3 = p * p * p;          /* p³ */
        seq_t pow  = (seq_t)1;
        uint64_t times = m / 3;          /* how many full triples we need */
        for (uint64_t k = 0; k < times; ++k)
            pow *= pow3;
        /* If m is not a multiple of 3 (should not happen after
           floor division) handle remaining factor. */
        if (m % 3 > 0)
            pow *= p;

        result *= pow;
    }

    return result;
}
