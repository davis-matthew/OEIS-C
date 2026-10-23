/*
 * A142102  Primes congruent to 7 mod 36.
 * https://oeis.org/A142102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime congruent to 7 mod 36 (search)
 * Range:     u8 n <= 5, u16 n <= 551, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    uint64_t count = 0;
    uint64_t candidate = 7;

    while (1)
    {
        if ((candidate % 36u) == 7u)
        {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate = next_prime(candidate + 1);
    }
}
