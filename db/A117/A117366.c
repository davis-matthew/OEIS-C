/*
 * A117366  a(n) = smallest prime greater than the largest prime dividing n.
 * https://oeis.org/A117366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime greater than the largest prime dividing n
 *            (a(1) is defined as 2)
 * Range:     u8 n <= 250, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9857 (2026-11-01)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

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
    /* The sequence is defined for n ≥ 1. For n = 1 the largest prime
       divisor is undefined, so we return the first prime, 2. */
    if (n <= 1)
        return (seq_t)2;

    /* The largest prime divisor of n (returns n if n itself is prime). */
    uint64_t lp = largest_prime_divisor((uint64_t)n);

    /* Find the smallest prime strictly greater than lp.  next_prime(p)
       returns the smallest prime > p, so we call it with lp directly. */
    uint64_t next = next_prime(lp);

    return (seq_t)next;
}
