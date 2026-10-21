/*
 * A067432  Number of ways to represent the n-th prime in form p*q+p+q, where
 *          p and q are primes (see A066938).
 * https://oeis.org/A067432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered pairs of primes (p,q) * such that the
 *            n‑th prime equals p*q + p + q. * * Equivalent to counting pairs
 *            of primes (p,q) with * (p+1)(q+1) = prime(n)+1, where p,q ≥ 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/is_prime.h"   /* bool is_prime(idx_t); */
#include "../utils/get_prime.h"  /* uint32_t get_prime(idx_t); */

seq_t a(idx_t n)
{
    /* Undefined for n==0; return 0 as a safe default. */
    if (n == 0) return 0;

    /* Retrieve the n‑th prime (1‑based). */
    uint32_t prime32 = get_prime(n);
    uint64_t prime = (uint64_t)prime32;

    /* Work with m = prime + 1. */
    uint64_t m = prime + 1;

    uint64_t count = 0;

    /* Enumerate divisors a of m with a ≥ 3 and a ≤ √m. */
    for (uint64_t a = 3; a * a <= m; ++a) {
        if (m % a != 0) continue;

        uint64_t b = m / a;
        if (b < 3) continue;          /* b-1 must be prime ≥ 2 */

        /* Check primality of p = a-1 and q = b-1. */
        if (is_prime((idx_t)(a - 1)) && is_prime((idx_t)(b - 1))) {
            ++count;
        }
    }

    /* Return the count as seq_t. */
    return (seq_t)count;
}
