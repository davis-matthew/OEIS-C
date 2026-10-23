/*
 * A124512  a(n) = A124508(A124508(A124508(n))).
 * https://oeis.org/A124512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9 * 2^(ω(n) + Ω(n) + 2) for n ≥ 2, a(1) = 1 where ω(n) =
 *            number of distinct prime factors (A001221), Ω(n) = total number
 *            of prime factors with multiplicity (A001222)
 * Range:     u8 n <= 3, u16 n <= 767, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, count_prime_factors, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_distinct_primes.h"
#include "../utils/count_prime_factors.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 1 */
    if (n <= 1u)
        return (seq_t)1;

    /* ω(n) – number of distinct prime factors. */
    idx_t distinct = count_distinct_primes((uint64_t)n);

    /* Ω(n) – total number of prime factors (with multiplicity). */
    /* count_prime_factors returns a seq_t, but its value fits in idx_t
       for all n that appear in the test suite. */
    idx_t total = (idx_t)count_prime_factors((seq_t)n);

    /* exponent for 2. */
    idx_t exp = distinct + total + 2u;

    /* 9 * 2^exp. */
    seq_t res = pow2(exp);  /* 2^exp in seq_t */
    res *= (seq_t)9;       /* multiply by 9 */

    return res;
}
