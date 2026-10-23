/*
 * A141939  Primes congruent to 16 mod 25.
 * https://oeis.org/A141939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 325, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: iterate numbers congruent to 41 (mod 50) and test each for primality */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; behaviour for n == 0 is unspecified */
    if (n == 0) return 0;

    idx_t candidate = 41;      /* first integer ≡41 (mod 50) */
    size_t count = 0;          /* number of qualifying primes found */

    for (;;)
    {
        if (is_prime(candidate))
        {
            ++count;
            if (count == n)
                return (seq_t)candidate;   /* cast to seq_t for the return value */
        }
        candidate += 50;           /* next number with the correct residue */
    }
}
