/*
 * A038915  Primes p such that 38 is a square mod p.
 * https://oeis.org/A038915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the n-th prime p such that 38 is a quadratic residue modulo p. */
#include <stdint.h>
#include <stdbool.h>
/* Utility functions from the build system */
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Unspecified behavior for n < 1; return 0. */
        return (seq_t)0;
    }

    idx_t count = 0;

    /* Handle p = 2 separately (2 divides 38, so 0 is a square). */
    count = 1;
    if (count == n) return (seq_t)2;

    /* Search odd primes starting from 3. */
    for (uint64_t p = 3; ; p += 2) {
        if (!is_prime(p)) continue;          /* Skip composites. */

        /* Prime 19 divides 38 -> 0 is a square, include it. */
        if (p == 19) {
            count++;
            if (count == n) return (seq_t)p;
            continue;
        }

        /* Legendre symbol (38/p) == 1 iff 38^((p-1)/2) mod p == 1. */
        uint64_t exp = (p - 1) / 2;
        seq_t result = mod_pow((seq_t)38, exp, (seq_t)p);
        if (result == (seq_t)1) {
            count++;
            if (count == n) return (seq_t)p;
        }
    }
}
