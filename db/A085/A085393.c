/*
 * A085393  Difference between the largest and the smallest prime factor of
 *          the greatest proper divisor of n.
 * https://oeis.org/A085393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest_prime_factor(n / smallest_prime_factor(n)) -
 *            smallest_prime_factor(n / smallest_prime_factor(n))
 * Range:     u8 n <= 1051, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 118 terms through n = 9109 (2026-11-01)
 * Helpers:   smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/smallest_prime_factor.h"

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
    if (n <= 1) return 0;
    uint64_t un = (uint64_t)n;
    uint64_t spf = smallest_prime_factor(un);
    if (spf == 0 || spf == un) return 0;          // prime or 1

    uint64_t d = un / spf;                        // greatest proper divisor
    if (d <= 1) return 0;

    uint64_t spf_d = smallest_prime_factor(d);
    if (spf_d == 0) return 0;                     // just in case

    uint64_t lpf_d = largest_prime_divisor(d);     // largest prime factor of d

    return (seq_t)(lpf_d - spf_d);
}
