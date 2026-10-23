/*
 * A167622  a(n) = n^3 mod n-th prime.
 * https://oeis.org/A167622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^3 mod nth prime
 * Range:     u8 n <= 57, u16 n <= 6647, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include <stdint.h>

seq_t a(idx_t n) {
    /* Obtain the n-th prime (1‑based).  get_prime returns a uint32_t,
       which is sufficient for any n examined in the tests. */
    uint64_t prime = (uint64_t)get_prime(n);

    /* Work in 64‑bit arithmetic to avoid overflows in intermediate
       calculations; the final result always fits in seq_t because
       it is less than the prime. */
    uint64_t n64   = (uint64_t)n;
    uint64_t nmod  = n64 % prime;     /* n modulo prime */
    uint64_t r     = (nmod * nmod) % prime; /* n^2 mod prime */
    r = (r * nmod) % prime;                 /* n^3 mod prime */

    return (seq_t)r;
}
