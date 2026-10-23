/*
 * A141983  Primes congruent to 7 mod 29.
 * https://oeis.org/A141983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime congruent to 7 (mod 29)
 * Range:     u8 n <= 3, u16 n <= 224, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is obtained by testing candidates 29*k+7 for primality. */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)            // safety, although offset starts at 1
        return 0;

    idx_t count = 0;
    idx_t candidate = 7;   // first prime in this residue class

    while (count < n)
    {
        if (is_prime(candidate))
        {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate += 29;
    }

    /* unreachable, but keep compiler happy */
    return 0;
}
