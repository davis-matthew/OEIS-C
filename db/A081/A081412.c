/*
 * A081412  Largest prime divisors of differences between consecutive primes.
 * https://oeis.org/A081412
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime divisor of prime(n+1) – prime(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External types declared by the build system are idx_t and seq_t.
 * They are not redefined here. */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* undefined for n<2, return 0 for safety */

    uint32_t p1 = get_prime(n);     /* n‑th prime (1‑indexed) */
    uint32_t p2 = get_prime(n + 1); /* (n+1)-th prime */

    uint64_t diff = (uint64_t)p2 - (uint64_t)p1;
    uint64_t lpf  = largest_prime_factor(diff); /* works for 64‑bit input */

    return (seq_t)lpf;              /* cast to the build's seq_t type */
}
