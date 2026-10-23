/*
 * A126956  Numbers k such that 3*k+2, 4*k+3 and 5*k+4 are primes.
 * https://oeis.org/A126956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k such that 3k+2, 4k+3 and 5k+4 are all
 *            prime
 * Range:     u8 n <= 6, u16 n <= 192, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence; for safety return 0 when n == 0 */
    if (n == 0)
        return (seq_t)0;

    idx_t count = 0;          /* how many suitable k's we have found so far   */
    idx_t k = 1;              /* candidate k to test (start at 1)              */

    for (;;) {
        /* construct the three numbers in the arithmetic progressions */
        uint64_t p1 = 3 * k + 2;
        uint64_t p2 = 4 * k + 3;
        uint64_t p3 = 5 * k + 4;

        /* check primality; use the generic helper that works for 64‑bit values */
        if (is_prime(p1) && is_prime(p2) && is_prime(p3)) {
            ++count;
            if (count == n)          /* the nth valid k has been found */
                return (seq_t)k;
        }
        ++k;                          /* next candidate */
    }
}
