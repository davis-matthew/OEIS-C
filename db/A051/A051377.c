/*
 * A051377  a(1)=1; for n > 1, a(n) = sum of exponential divisors (or
 *          e-divisors) of n.
 * https://oeis.org/A051377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult
 * Formula:   a(n) = ∏_{p^e‖n} (∑_{d‖e} p^d) where d runs over the positive
 *            divisors of e. a(1)=1
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sum_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sum_prime_power.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;  /* a(1)=1 */

    uint64_t m = (uint64_t)n;
    seq_t result = 1;

    /* Handle factor 2 separately. */
    int e = 0;
    while ((m & 1ULL) == 0) {
        ++e;
        m >>= 1;
    }
    if (e > 0)
        result *= sum_prime_power(2ULL, e);

    /* Factor odd primes. */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                ++e;
                m /= p;
            }
            result *= sum_prime_power(p, e);
        }
    }

    /* If something remains, it is a prime factor with exponent 1. */
    if (m > 1)
        result *= (seq_t)m;   /* sum = p for exponent 1 */

    return result;
}
