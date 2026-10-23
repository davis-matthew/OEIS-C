/*
 * A023575  Number of distinct prime divisors of prime(n)+3.
 * https://oeis.org/A023575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = omega(prime(n)+3)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* Retrieve the n-th prime; count_distinct_primes returns
       the number of distinct prime factors of its argument. */
    uint64_t p = (uint64_t)get_prime(n);  /* n is 1‑based */
    uint64_t val = p + 3ULL;
    return (seq_t)count_distinct_primes(val);
}
