/*
 * A135461  a(n) = 1 if n is the norm of an Eisenstein prime (see A055664)
 *          otherwise 0.
 * https://oeis.org/A135461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is the norm of an Eisenstein prime (3, primes ≡1
 *            mod3, or squares of primes ≡2 mod3); otherwise a(n) = 0
 * Range:     u8 n <= 8316, u16 n <= 8316, u32 n <= 8316, u64 n <= 8316,
 *            u128 n <= 8316, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Integer square root for uint64_t (floor). */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = ((uint64_t)1 << 32); /* sqrt(2^64-1) < 2^32 */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid <= n / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Small trivial cases */
    if (n == 3)
        return (seq_t)1;
    if (n < 2)
        return (seq_t)0;

    /* Check if n is a prime ≡ 1 (mod 3) */
    if (is_prime(n) && n % (idx_t)3 == (idx_t)1)
        return (seq_t)1;

    /* Check if n is the square of a prime ≡ 2 (mod 3) */
    uint64_t m = isqrt_u64((uint64_t)n);
    if (m * m == (uint64_t)n && is_prime(m) && m % (uint64_t)3 == (uint64_t)2)
        return (seq_t)1;

    return (seq_t)0;
}
