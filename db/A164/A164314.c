/*
 * A164314  Largest prime factor of n^2 - 2.
 * https://oeis.org/A164314
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime factor of n^2 - 2
 * Range:     u8 n <= 18, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/largest_prime_divisor.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 2; for n < 2 the behavior is not defined. */
    if (n < 2) {
        return (seq_t)0;
    }

    /* Compute n^2 - 2 in 64‑bit arithmetic (sufficient for the test range
       and compatible with any seq_t size). */
    uint64_t N = (uint64_t)n;
    uint64_t val = N * N - 2u;

    /* Get the largest prime divisor using the helper. */
    uint64_t lpf = largest_prime_divisor(val);

    return (seq_t)lpf;
}
