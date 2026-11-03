/*
 * A180529  Primes that become a different prime under the mapping 1 <=> 6.
 * https://oeis.org/A180529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th prime p such that swapping the digits 1 and 6 in
 *            p yields a different prime. This implementation searches
 *            sequentially for primes, swapping digits as needed, until the
 *            desired index is found
 * Range:     u8 n <= 6, u16 n <= 623, u32 n <= 2107, u64 n <= 2107,
 *            u128 n <= 2107, big any n
 * Validated: standard, pass, 61 terms through n = 2089 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Swap the digits 1 and 6 in a 64‑bit integer.
   Digits other than 1 or 6 remain unchanged. */
static uint64_t swap16_uint64(uint64_t x)
{
    uint64_t y   = 0;
    uint64_t pow = 1;
    while (x > 0)
    {
        uint64_t d = x % 10;
        if (d == 1)
            d = 6;
        else if (d == 6)
            d = 1;
        y += d * pow;
        pow *= 10;
        x  /= 10;
    }
    return y;
}

/* Return the nth term of A180529.
   n is 1‑based; a(1) is 17. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined, defensive return */

    idx_t candidate = 2;      /* candidate prime to test */
    idx_t found     = 0;      /* number of qualifying primes found */

    while (found < n)
    {
        if (is_prime(candidate))
        {
            uint64_t mapped = swap16_uint64(candidate);
            if (mapped != candidate && is_prime(mapped))
            {
                ++found;
                if (found == n)
                    return (seq_t)candidate;
            }
        }
        ++candidate;
    }

    return 0; /* unreachable for valid inputs */
}
