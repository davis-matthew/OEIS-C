/*
 * A141871  Primes congruent to 6 mod 19.
 * https://oeis.org/A141871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 357, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n‑th prime congruent to 6 modulo 19 (found by trial) */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* 6 + 19*k gives all integers ≡ 6 (mod 19).  The first such prime is 101. */
    idx_t candidate = 101;  /* first prime in the sequence */
    idx_t count = 0;        /* number of primes found so far */

    while (count < n)
    {
        if (is_prime(candidate))
        {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate += 19;    /* next integer in the arithmetic progression */
    }

    /* The loop is guaranteed to return, but provide a fallback. */
    return 0;
}
