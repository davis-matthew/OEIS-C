/*
 * A142563  Primes congruent to 33 mod 53.
 * https://oeis.org/A142563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 125, u32 n <= 80, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142563: primes congruent to 33 mod 53
   a(n) = the n‑th prime p such that p ≡ 33 (mod 53).  The
   algorithm searches successive candidates (33 + 53*k) and
   tests primality with is_prime_big.  This yields the correct
   ordinal list for any n ≥ 1. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n is 1‑based. For safety, handle empty request. */
    if (n == 0)
        return 0;

    seq_t candidate = 33;        /* first number ≡ 33 (mod 53) */
    idx_t   count    = 0;        /* how many primes found so far */

    for (;;) {
        if (is_prime_big(candidate)) {
            ++count;
            if (count == n)
                return candidate;        /* found the nth prime */
        }
        candidate += 53;            /* next candidate in the arithmetic progression */
    }
}
