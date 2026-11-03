/*
 * A217434  n divided by the product of all its prime divisors smaller than
 *          the largest prime divisor.
 * https://oeis.org/A217434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*A006530(n)/A007947(n) (largest prime divisor * n /
 *            product of distinct prime divisors)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9498 (2026-11-01)
 * Helpers:   smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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
    if (n == 1)                       /* a(1) = 1 */
        return (seq_t)1;

    uint64_t nn   = (uint64_t)n;      /* work with 64‑bit values for factorisation */
    seq_t   rad   = 1;                /* product of distinct prime divisors */

    /* factor n by repeatedly extracting the smallest prime factor */
    while (nn > 1) {
        uint64_t p = smallest_prime_factor(nn);
        rad *= (seq_t)p;             /* include each prime once in rad */
        while (nn % p == 0)
            nn /= p;                 /* strip all copies of the current prime */
    }

    uint64_t gpf = largest_prime_divisor(n);   /* largest prime divisor (A006530) */

    /* a(n) = n * gpf / rad  */
    seq_t result = ((seq_t)n) * (seq_t)gpf / rad;
    return result;
}
