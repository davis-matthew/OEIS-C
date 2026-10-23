/*
 * A161656  The largest multiple of {the sum of the distinct prime divisors of
 *          n} that is <=n.
 * https://oeis.org/A161656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest multiple of (sum of distinct prime divisors of n)
 *            that is <= n. For n = 1, a(n) = 0
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Sum of distinct prime divisors of n */
    uint64_t u = (uint64_t)n;
    if (u == 1) return (seq_t)0;   /* no primes, define a(1)=0 */

    uint64_t sum = 0;
    uint64_t temp = u;

    /* Trial division; sufficient for n up to at least 10^12 in a few ms */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            sum += p;              /* distinct prime */
            while (temp % p == 0) temp /= p;
        }
    }
    /* If residual > 1, it's a prime factor > sqrt(original) */
    if (temp > 1) sum += temp;

    /* Largest multiple of sum not exceeding n */
    seq_t quotient = (seq_t)n / (seq_t)sum;
    return quotient * (seq_t)sum;
}
