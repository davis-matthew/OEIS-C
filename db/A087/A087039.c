/*
 * A087039  If n is prime then 1 else 2nd largest prime factor of n.
 * https://oeis.org/A087039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n is prime; otherwise the second largest prime
 *            factor of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;          /* a(1) = 1 (n = 0 is never queried) */

    if (is_prime(n))
        return (seq_t)1;          /* primes yield 1 */

    uint64_t m = (uint64_t)n;
    uint64_t lp1 = largest_prime_factor(m);   /* largest prime factor */
    uint64_t rem = m / lp1;                    /* remove one occurrence of lp1 */
    uint64_t lp2 = largest_prime_factor(rem);  /* second largest (may equal lp1) */
    return (seq_t)lp2;
}
