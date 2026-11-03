/*
 * A230774  Number of primes less than first prime above square root of n.
 * https://oeis.org/A230774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9889 (2026-11-01)
 * Helpers:   is_prime, isqrt, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/isqrt.h"
#include "../utils/prime_pi.h"

/* Sequence A230774:
 * a(n) = π(p) where p is the smallest prime ≥ ceil(sqrt(n)),
 * i.e. the number of primes ≤ that prime.
 * For example, n = 5…9: ceil(sqrt(n)) = 3, π(3) = 2. */

seq_t a(idx_t n)
{
    if (n == 0u) return 0u;

    /* ceil sqrt: start with floor sqrt */
    idx_t s = isqrt(n);
    if (s * s < n) ++s;            /* now s = ceil(sqrt(n)) */

    /* first prime ≥ s */
    idx_t p = s;
    while (!is_prime(p))
        ++p;

    return prime_pi(p);
}
