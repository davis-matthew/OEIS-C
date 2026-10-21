/*
 * A076605  Largest prime divisor of n^2 - 1.
 * https://oeis.org/A076605
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max(largest prime factor of n-1, largest prime factor of
 *            n+1)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use the helper function largest_prime_factor from the utils library. */
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    /* Offset is 2, but return 0 for n < 2 to indicate undefined terms. */
    if (n < 2)
        return 0;

    /* Compute largest prime factor of n-1. */
    uint64_t lp1 = 1;            /* 1 is used for n-1 = 1 (when n == 2). */
    if (n > 2)                   /* n-1 >= 2 */
        lp1 = largest_prime_factor((uint64_t)(n - 1));

    /* Compute largest prime factor of n+1. */
    uint64_t lp2;
    if (n == UINT64_MAX)
    {
        /* n+1 would overflow; in this unlikely case we ignore the second term. */
        lp2 = 1;
    }
    else
    {
        lp2 = largest_prime_factor((uint64_t)(n + 1));
    }

    /* Return the maximum of the two largest prime factors. */
    uint64_t result = lp1 > lp2 ? lp1 : lp2;
    return (seq_t)result;
}
