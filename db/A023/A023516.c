/*
 * A023516  Number of distinct prime divisors of prime(n)*prime(n-1) - 1.
 * https://oeis.org/A023516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct prime divisors of prime(n)*prime(n-1)
 *            - 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          (prime(0) is defined as 1 for n = 1) */
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* n >= 1 for valid queries.  For n < 1 we simply return 0. */
    if (n < 1)
        return (seq_t)0;

    /* Fetch the nth prime.  For n == 1, prime(0) is treated as 1. */
    uint64_t p_n   = get_prime(n);
    uint64_t p_nm1 = (n == 1) ? 1U : get_prime(n - 1);

    /* Compute M = prime(n) * prime(n-1) - 1.
       The multiplication fits in 64‑bit for n up to at least 10^6. */
    uint64_t m = (uint64_t)p_n * p_nm1;
    m -= 1ULL;

    /* Count distinct prime factors of M.  The helper function returns int. */
    return (seq_t)count_distinct_primes(m);
}
