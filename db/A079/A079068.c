/*
 * A079068  Largest prime less than greatest prime factor of n but not
 *          dividing n, or 1 if no such prime exists.
 * https://oeis.org/A079068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 262, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 103 terms through n = 9031 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A079068: a(n) = largest prime < gpf(n) not dividing n; a(1)=1 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

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
    if (n == 1) return (seq_t)1;

    uint64_t gpf = largest_prime_divisor(n);
    if (gpf <= 2) return (seq_t)1;

    long long p = (long long)gpf - 1;          // start just below the greatest prime factor
    for (; p >= 2; --p) {
        if (is_prime((idx_t)p) && (n % (idx_t)p != 0))
            return (seq_t)p;
    }
    return (seq_t)1;
}
