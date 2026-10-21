/*
 * A093191  Primes congruent to 4 mod 13.
 * https://oeis.org/A093191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 546, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 4 (mod 13) ≡ 17 (mod 26). */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n==0, return 0 safely */

    idx_t count     = 0;
    idx_t candidate = 17;         /* first candidate in progression 17 + 26k */

    for (;; candidate += 26)      /* iterate over numbers ≡ 17 mod 26 */
    {
        if (is_prime(candidate))
        {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
    }
}
