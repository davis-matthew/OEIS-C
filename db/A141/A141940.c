/*
 * A141940  Primes congruent to 17 mod 25.
 * https://oeis.org/A141940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime congruent to 17 mod 25 (search)
 * Range:     u8 n <= 3, u16 n <= 331, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t cand = 17;
    for (;; cand += 25)
    {
        if (is_prime(cand))
        {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
    }
}
