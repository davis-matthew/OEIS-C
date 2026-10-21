/*
 * A087624  a(n)=0 if n is prime, A001221(n) otherwise.
 * https://oeis.org/A087624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A001221(n) * (1 - is_prime(n)) i.e. number of distinct
 *            prime divisors of n, except 0 when n itself is prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/count_distinct_primes.h"

/* Computes OEIS sequence A087624.
   Return type seq_t and index type idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* 1 is not prime; count_distinct_primes(1) == 0, so a(1) == 0. */
    if (is_prime(n))
        return 0;
    /* count_distinct_primes returns an int; cast to seq_t for the result. */
    return (seq_t)count_distinct_primes((uint64_t)n);
}
