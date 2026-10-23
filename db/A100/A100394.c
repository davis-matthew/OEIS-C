/*
 * A100394  a(n) is the subscript of the greatest prime factor of (2*prime(n)
 *          + 1).
 * https://oeis.org/A100394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = π( largest prime factor of (2*prime(n)+1) )
 * Range:     u8 n <= 31, u16 n <= 4669, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, greatest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A100394 */

/* Include the required helper functions */
#include "../utils/get_prime.h"
#include "../utils/greatest_prime_factor.h"

seq_t a(idx_t n)
{
    /* For OEIS the sequence is defined starting at n = 1. */
    if (n < 1)
        return (seq_t)0;

    /* Get the n-th prime q. */
    seq_t q = get_prime((uint64_t)n);          /* q = prime(n) */

    /* Compute m = 2*q + 1. */
    seq_t m = q * 2 + 1;

    /* Find the greatest prime factor r of m. */
    seq_t r = greatest_prime_factor(m);       /* r = A006530(m) */

    /* Now find the index of r among the primes: pi(r). */
    uint64_t idx = 1;                          /* index counter */
    while (1) {
        seq_t p = get_prime(idx);             /* p = prime(idx) */
        if (p == r)                           /* found the prime */
            return (seq_t)idx;                /* a(n) = idx */
        if (p > r)                            /* safety out-of-range guard */
            break;
        ++idx;
    }

    /* In theory we should never reach this point; return 0 as a safeguard. */
    return (seq_t)0;
}
