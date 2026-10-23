/*
 * A140442  Primes congruent to 9 mod 14.
 * https://oeis.org/A140442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: nth prime p such that p % 14 == 9 (offset 1) */
#include <stdint.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined offset; return 0 for safety */
    idx_t count = 0;
    uint64_t candidate = 2; /* start just below the first prime */
    for (;;)
    {
        candidate = next_prime(candidate);
        if (candidate % 14 == 9)
        {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
    }
}
