/*
 * A074399  a(n) is the largest prime divisor of n(n+1).
 * https://oeis.org/A074399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max(LargestPrimeFactor(n), LargestPrimeFactor(n+1)) where
 *            LargestPrimeFactor(m) is the largest prime that divides m. The
 *            largest prime factor is obtained by simple trial division up to
 *            sqrt(m). For m < 2 the function returns m itself (so that a(1) =
 *            2)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Return the largest prime factor of a 64‑bit unsigned integer.
   For numbers smaller than 2 it returns the number itself. */
static seq_t largest_prime_factor_uint64(uint64_t m)
{
    if (m < 2u)
        return (seq_t)m;

    seq_t max_factor = 1;

    /* remove factors of 2 */
    while ((m & 1ULL) == 0) {
        max_factor = 2;
        m >>= 1;
    }

    /* trial division by odd numbers */
    for (uint64_t d = 3; d <= m / d; d += 2) {
        while (m % d == 0) {
            max_factor = (seq_t)d;
            m /= d;
        }
    }

    /* if anything is left it is a prime factor > sqrt(original m) */
    if (m > 1)
        max_factor = (seq_t)m;

    return max_factor;
}

/* OEIS A074399 ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;
    seq_t p1 = largest_prime_factor_uint64(x);
    seq_t p2 = largest_prime_factor_uint64(x + 1ULL);
    return (p1 > p2) ? p1 : p2;
}
