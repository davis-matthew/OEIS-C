/*
 * A070801  Largest prime <= sigma(n): a(n) = prevprime(sigma(n)), where
 *          prevprime(n) = A007917(n), the largest prime less than or equal to
 *          n.
 * https://oeis.org/A070801
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = largest prime <= sigma(n)
 * Range:     u8 n <= 107, u16 n <= 16385, u32 n <= 16385, u64 n <= 16385,
 *            u128 n <= 16385, big any n
 * Validated: standard, pass, 84 terms through n = 9562 (2026-11-01)
 * Helpers:   is_prime, sigma_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/sigma_u64.h"

seq_t a(idx_t n)
{
    /* Compute the sum of divisors of n. */
    seq_t sigma = sigma_u64((uint64_t)n);

    /* The sum of divisors of n with n <= 10000 fits in a 64‑bit unsigned
       integer, so we can search downward in that range. */
    unsigned long long p = (unsigned long long)sigma;

    /* Find the greatest prime <= sigma.  n >= 2 guarantees that
       sigma >= 3, so 2 is always a valid fallback. */
    while (p >= 2) {
        if (is_prime(p))
            return (seq_t)p;
        if (p == 2)
            break;
        --p;
    }
    return (seq_t)2;
}
