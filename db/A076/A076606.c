/*
 * A076606  Min { largest prime factor of n-1, largest prime factor of n+1 }.
 * https://oeis.org/A076606
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Min{largest prime factor of n-1, largest prime factor of
 *            n+1}
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    /* For n < 3 the offset is not defined; return 0. */
    if (n < 3)
        return (seq_t)0;

    /* Compute the largest prime factor of n-1 and n+1 */
    seq_t lp1 = (seq_t)largest_prime_factor((uint64_t)n - 1);
    seq_t lp2 = (seq_t)largest_prime_factor((uint64_t)n + 1);

    /* Return the smaller of the two */
    return (lp1 < lp2) ? lp1 : lp2;
}
