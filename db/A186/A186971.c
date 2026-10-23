/*
 * A186971  Maximal cardinality of a subset of {1, 2, ..., n} containing n and
 *          having pairwise coprime elements.
 * https://oeis.org/A186971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n (n < 4) a(n) = pi(n) - omega(n) + 2 (n >= 4) pi(n) :
 *            number of primes <= n (A000720) omega(n): number of distinct
 *            prime divisors of n (A001221)
 * Range:     u8 n <= 1612, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/prime_pi.h"          /* seq_t prime_pi(idx_t) */
#include "../utils/count_distinct_primes.h" /* int count_distinct_primes(uint64_t) */

seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)n;

    seq_t pi = prime_pi(n);
    seq_t omega = (seq_t)count_distinct_primes((uint64_t)n);

    return pi - omega + 2;
}
