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
 * Helpers:   largest_prime_divisor, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/largest_prime_divisor.h"
#include "../utils/next_prime.h"

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
