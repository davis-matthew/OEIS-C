/*
 * A039736  a(n) = number of primes q<p having (p mod q) = 2, where p = n-th
 *          prime.
 * https://oeis.org/A039736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct prime factors of prime(n) – 2
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   count_distinct_primes, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* n is a 1‑based index; obtain the nth prime (uint32_t) */
    uint64_t p   = (uint64_t)get_prime(n);   /* the nth prime */
    uint64_t m   = p - 2;                    /* number to factorize */
    int      cnt = count_distinct_primes(m); /* distinct prime factors of m */

    return (seq_t)cnt;
}
