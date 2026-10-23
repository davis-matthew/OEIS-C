/*
 * A034444  a(n) is the number of unitary divisors of n (d such that d divides
 *          n, gcd(d, n/d) = 1).
 * https://oeis.org/A034444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, mult
 * Formula:   a(n) = 2^(number of distinct prime factors of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalently a(n) = Sum_{d|n} |mu(d)|) */

#include <stdint.h>
#include <stddef.h>
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. */
    if (n == 0) return 0;   /* safety, not part of the OEIS offset */

    /* Count the distinct prime factors of n. */
    size_t distinct = count_distinct_primes((uint64_t)n);

    /* Compute 2^distinct using only seq_t arithmetic. */
    seq_t result = 1;
    for (size_t i = 0; i < distinct; ++i)
        result *= 2;

    return result;
}
