/*
 * A230774  Number of primes less than first prime above square root of n.
 * https://oeis.org/A230774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, floor_isqrt_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_isqrt_u64.h"
#include "../utils/is_prime.h"
#include "../utils/compute_pi.h"

/* Sequence A230774:
 * a(n) = π(p) where p is the smallest prime ≥ ceil(sqrt(n)),
 * i.e. the number of primes ≤ that prime.
 * For example, n = 5…9: ceil(sqrt(n)) = 3, π(3) = 2. */

seq_t a(idx_t n)
{
    if (n == 0u) return 0u;

    /* ceil sqrt: start with floor sqrt */
    idx_t s = floor_isqrt_u64(n);
    if (s * s < n) ++s;            /* now s = ceil(sqrt(n)) */

    /* first prime ≥ s */
    idx_t p = s;
    while (!is_prime(p))
        ++p;

    return compute_pi(p);
}
