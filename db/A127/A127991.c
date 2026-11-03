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
 * Validated: standard, pass, 90 terms through n = 9890 (2026-11-01)
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
