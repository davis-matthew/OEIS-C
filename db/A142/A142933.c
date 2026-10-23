/*
 * A142933  Primes congruent to 17 mod 64.
 * https://oeis.org/A142933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime number that is ≡ 17 (mod 64)
 * Range:     u8 n <= 1, u16 n <= 200, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t candidate = 17;
    while (count < n)
    {
        if (is_prime(candidate))
        {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate += 64;
    }
    /* Should never happen for n >= 1 */
    return (seq_t)0;
}
