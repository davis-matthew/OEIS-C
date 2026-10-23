/*
 * A127991  2*n^3 - 2*n + 9 divided by 3*largest prime factor.
 * https://oeis.org/A127991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 - 2*n + 9) / (3*largest_prime_factor(2*n^3 - 2*n +
 *            9))
 * Range:     u8 n <= 16, u16 n <= 196, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/largest_prime_divisor.h"

seq_t a(idx_t n)
{
    /* compute 2*n^3 - 2*n + 9 in 64‑bit arithmetic */
    uint64_t n64 = (uint64_t)n;
    uint64_t val = 2ULL * n64 * n64 * n64 - 2ULL * n64 + 9ULL;

    /* largest prime factor */
    uint64_t lp = largest_prime_divisor(val);

    /* quotient gives the sequence value */
    uint64_t denom = 3ULL * lp;
    uint64_t res64 = val / denom;

    return (seq_t)res64;
}
