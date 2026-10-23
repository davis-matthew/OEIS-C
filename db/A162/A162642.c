/*
 * A162642  Number of odd exponents in the canonical prime factorization of n.
 * https://oeis.org/A162642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of odd exponents in the canonical prime
 *            factorization of n
 * Range:     u8 n <= 8287, u16 n <= 8287, u32 n <= 8287, u64 n <= 8287,
 *            u128 n <= 8287, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1: a(1) = 0. For n <= 1 we return 0. */
    if (n <= 1)
        return 0;

    uint64_t x = (uint64_t)n;

    /* For 64‑bit numbers the maximal number of distinct prime factors
       is below 16. A fixed small buffer suffices. */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;

    /* Factor the number; factor_u64 writes the primes, their exponents
       and the count into the provided arrays. */
    factor_u64(x, primes, exps, &cnt);

    seq_t odd_count = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        if ((exps[i] & 1ULL) != 0)     /* exponent is odd? */
            odd_count += 1;
    }

    return odd_count;
}
