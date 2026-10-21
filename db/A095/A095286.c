/*
 * A095286  Primes in whose binary expansion the number of 1 bits is > 1 +
 *          number of 0 bits.
 * https://oeis.org/A095286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 3722, u32 n <= 8553, u64 n <= 8553,
 *            u128 n <= 8553, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes whose binary representation has more 1‑bits than
   0‑bits + 1.  This enumerates all such primes and returns the nth one
   (OEIS A095286). */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Count how many qualifying primes we have found so far. */
    idx_t found = 0;
    /* Candidate prime to test.  2 never satisfies the inequality, so
       start with the smallest odd prime. */
    idx_t p = 3;

    /* Keep scanning numbers until we reach the desired nth term. */
    for (;; ++p)
    {
        if (!is_prime(p))
            continue;

        /* Count 1‑bits and 0‑bits in the binary expansion of p. */
        idx_t x = p, bits = 0, ones = 0;
        while (x)
        {
            if (x & 1U)
                ++ones;
            x >>= 1;
            ++bits;
        }
        idx_t zeros = bits - ones;

        /* Check the condition 1‑bits > 0‑bits + 1. */
        if (ones > zeros + 1U)
        {
            ++found;
            if (found == n)
                return (seq_t)p;   /* cast to the output type */
        }
    }
}
