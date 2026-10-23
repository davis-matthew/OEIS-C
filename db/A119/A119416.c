/*
 * A119416  n * (smallest prime greater than largest prime factor of n).
 * https://oeis.org/A119416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * next_prime(largest_prime_factor(n))
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_factor, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    /* Convert index to 64‑bit integer */
    uint64_t n64 = (uint64_t)n;

    /* Largest prime factor; for n = 1 we use 1 */
    uint64_t lpf = (n64 <= 1) ? 1 : largest_prime_factor(n64);

    /* Smallest prime greater than lpf */
    uint64_t nextp = next_prime(lpf);

    /* Compute result as seq_t */
    return (seq_t)n64 * (seq_t)nextp;
}
