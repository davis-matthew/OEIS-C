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
 * Validated: standard, pass, 87 terms through n = 8462 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute ∑_{d|e} p^d for a prime power p^e (e ≥ 1).  The sum is taken
   over all positive divisors d of e.  */
static inline seq_t sum_prime_power(uint64_t p, int e)
{
    seq_t sum   = 0;
    seq_t power = 1;
    for (int d = 1; d <= e; ++d) {
        power *= (seq_t)p;          /* p^d */
        if (e % d == 0)
            sum += power;
    }
    return sum;
}

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
