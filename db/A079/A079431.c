/*
 * A079431  Least m>n with same smallest odd prime factor as n.
 * https://oeis.org/A079431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   smallest_odd_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute the least m > n having the same smallest odd prime factor
    as n.  The routine is a straightforward search that checks each
    candidate integer using the utility function
    smallest_odd_prime_factor(uint64_t v).  For powers of two the
    smallest odd prime factor is defined to be 1, so the next term is
    simply the next power of two.  This implementation does not rely
    on any precomputed tables and works for any idx_t value.
*/
#include <stdint.h>
#include "../utils/smallest_odd_prime_factor.h"

seq_t a(idx_t n)
{
    /* Determine the smallest odd prime factor of n (1 if n is a power
       of 2). */
    uint64_t p = smallest_odd_prime_factor((uint64_t)n);

    /* Powers of two (p == 1) are handled specially: the next term is
       the next power of two. */
    if (p == 1) {
        /* n itself must be a power of two; return the next one. */
        return (seq_t)(n << 1);
    }

    /* For other n, search forward for the first integer having the
       same smallest odd prime factor. */
    idx_t m = n + 1;
    for (;;) {
        if (smallest_odd_prime_factor((uint64_t)m) == p)
            return (seq_t)m;
        ++m;
    }
}
