/*
 * A142497  Primes congruent to 35 mod 51.
 * https://oeis.org/A142497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 213, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: nth prime ≡ 35 (mod 51).  Computed by brute‑force search. */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 0; ; ++k)
    {
        idx_t candidate = 51 * k + 35;
        if (is_prime(candidate))
        {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
    }
}
