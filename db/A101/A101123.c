/*
 * A101123  Numbers k for which 7*k + 11 is prime.
 * https://oeis.org/A101123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 825, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isprime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for integers k such that 7*k + 11 is prime.
   The function returns the nth such k (with a(1) = 0). */

#include <stdbool.h>
#include "../utils/isprime_big.h"

seq_t a(idx_t n)
{
    if (n < 1)   /* not used but safe */
        return 0;

    idx_t  found = 0;      /* how many primes found */
    seq_t  k     = 0;      /* current candidate k */

    while (true)
    {
        seq_t candidate = k * 7 + 11;
        if (isprime_big(candidate))
        {
            ++found;
            if (found == n)
                return k;
        }
        ++k;
    }
}
