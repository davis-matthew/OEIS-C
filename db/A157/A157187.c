/*
 * A157187  Number of ways to write n as p*q-(p+q) with primes p <= q.
 * https://oeis.org/A157187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A157187 (Number of ways to write n as p*q-(p+q) with primes p <= q)
 *  The function computes a(n) by factorising n+1 into
 *  (p-1)*(q-1) where p and q are primes, and counting each unordered pair.
 *  a(n) is the number of divisors d of n+1 such that
 *      d <= (n+1)/d   and   d+1 and (n+1)/d+1 are prime.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t t = n + 1;            /* (p-1)*(q-1) = n+1 */

    for (idx_t d = 1; d <= t / d; ++d) {
        if (t % d != 0) continue;

        idx_t e = t / d;        /* e = (q-1) */

        /* Check primality of p = d+1 and q = e+1 */
        if (is_prime(d + 1) && is_prime(e + 1)) {
            /* Count only once: p <= q -> d <= e (already ensured) */
            ++count;
        }
    }

    return (seq_t)count;
}
