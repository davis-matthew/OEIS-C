/*
 * A153307  Numbers k such that 14*k+3 is not prime.
 * https://oeis.org/A153307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth integer k such that 14*k+3 is composite. */
/* The function simply counts such k’s until the desired rank is reached. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t k = 0;

    while (found < n)
    {
        /* Compute 14*k+3 in a wider type to avoid overflow during multiplication. */
        uint64_t tmp = (uint64_t)14 * (uint64_t)k + 3;
        seq_t val = (seq_t)tmp;

        if (!is_prime_big(val))
        {
            ++found;
            if (found == n)
                return k;
        }
        ++k;
    }

    return k;  /* unreachable, keeps the compiler happy */
}
