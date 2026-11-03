/*
 * A080213  a(n) = binomial(n, greatest prime factor of n).
 * https://oeis.org/A080213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n, largest_prime_divisor(n))
 * Range:     u8 n <= 13, u16 n <= 20, u32 n <= 37, u64 n <= 73, u128 n <= 133,
 *            big any n
 * Validated: standard, pass, 59 terms through n = 2898 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"

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

seq_t a(idx_t n) {
    if (n <= 1) return 1;            /* a(1) = 1 by definition */
    uint64_t p = largest_prime_divisor(n); /* greatest prime factor of n */
    return binomial(n, p);           /* binomial(n, p) is computed in seq_t */
}
