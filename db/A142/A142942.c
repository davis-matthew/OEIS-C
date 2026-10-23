/*
 * A142942  Primes congruent to 39 mod 64.
 * https://oeis.org/A142942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 194, u32 n <= 195, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth prime that is congruent to 39 modulo 64.
   Sequence A142942: primes p such that p ≡ 39 (mod 64). */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* Undefined for 0, but return 0 */

    idx_t count = 0;
    seq_t cand = (seq_t)39;           /* 39 mod 64 */
    while (true)
    {
        cand += (seq_t)64;            /* Next number ≡ 39 (mod 64) */
        if (is_prime_big(cand))      /* Test primality */
        {
            count++;
            if (count == n)
                return cand;          /* Found the nth term */
        }
    }
}
