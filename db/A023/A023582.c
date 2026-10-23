/*
 * A023582  Number of distinct prime divisors of 2*prime(n)-1.
 * https://oeis.org/A023582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   count_distinct_primes, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of distinct prime divisors of (2*prime(n)-1)
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    /* Fetch the n-th prime (1‑based index) */
    uint32_t p = get_prime(n);
    /* Compute 2*prime(n)-1 in 64‑bit to stay safe */
    uint64_t val = ((uint64_t)2 * (uint64_t)p) - 1ULL;
    /* Count distinct prime factors */
    int cnt = count_distinct_primes(val);
    return (seq_t)cnt;
}
