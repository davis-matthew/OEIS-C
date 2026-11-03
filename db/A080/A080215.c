/*
 * A080215  Binomial(greatest prime factor of n, smallest odd prime factor of
 *          n).
 * https://oeis.org/A080215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(greatest_prime_factor(n),
 *            smallest_odd_prime_factor(n))
 * Range:     u8 n <= 38, u16 n <= 144, u32 n <= 472, u64 n <= 1816,
 *            u128 n <= 6682, big any n
 * Validated: standard, pass, 100 terms through n = 9982 (2026-11-01)
 * Helpers:   binomial, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/largest_prime_factor.h"

/* compute the smallest odd prime factor of n; if none, return 1 */
static idx_t smallest_odd_prime_factor(idx_t n)
{
    if (n <= 2) return 1;            /* n = 1 or 2 has no odd prime factor */
    uint64_t m = n;
    while ((m & 1) == 0)             /* strip powers of 2 */
        m >>= 1;
    if (m == 1) return 1;            /* n was a power of 2 */

    /* m is now odd. Find its smallest prime divisor. */
    for (uint64_t p = 3; p * p <= m; p += 2)
    {
        if (m % p == 0)
            return (idx_t)p;
    }
    /* m itself is prime and odd */
    return (idx_t)m;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* out of range; not used */
    uint64_t g = largest_prime_factor(n);          /* greatest prime factor of n (returns 1 for n = 1) */
    idx_t sop = smallest_odd_prime_factor(n);
    return binomial((idx_t)g, sop);
}
