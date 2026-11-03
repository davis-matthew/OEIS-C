/*
 * A164314  Largest prime factor of n^2 - 2.
 * https://oeis.org/A164314
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime factor of n^2 - 2
 * Range:     u8 n <= 18, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9797 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return the largest prime divisor of m (m >= 2). */
static uint64_t largest_prime_divisor(uint64_t m)
{
    uint64_t largest = 0;
    /* factor 2 */
    while ((m & 1ULL) == 0) {
        largest = 2;
        m >>= 1;
    }
    /* odd factors */
    for (uint64_t f = 3; f <= m / f; f += 2) {
        if (m % f == 0) {
            largest = f;
            while (m % f == 0)
                m /= f;
        }
    }
    if (m > 1)            /* remaining m is prime */
        largest = m;
    return largest;
}

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
