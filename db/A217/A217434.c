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
 * Helpers:   _smallest_prime_factor, largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/largest_prime_divisor.h"

seq_t a(idx_t n)
{
    if (n == 1)                       /* a(1) = 1 */
        return (seq_t)1;

    uint64_t nn   = (uint64_t)n;      /* work with 64‑bit values for factorisation */
    seq_t   rad   = 1;                /* product of distinct prime divisors */

    /* factor n by repeatedly extracting the smallest prime factor */
    while (nn > 1) {
        uint64_t p = _smallest_prime_factor(nn);
        rad *= (seq_t)p;             /* include each prime once in rad */
        while (nn % p == 0)
            nn /= p;                 /* strip all copies of the current prime */
    }

    uint64_t gpf = largest_prime_divisor(n);   /* largest prime divisor (A006530) */

    /* a(n) = n * gpf / rad  */
    seq_t result = ((seq_t)n) * (seq_t)gpf / rad;
    return result;
}
