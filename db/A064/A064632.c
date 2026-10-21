/*
 * A064632  Smallest prime p such that n = (p-1)/(q-1) for some prime q.
 * https://oeis.org/A064632
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 823, u32 n <= 7828, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest prime p such that n = (p-1)/(q-1) for some prime q (OEIS A064632). */
/* The algorithm iterates over successive primes q, computing p = n*(q-1)+1
   and returning the first p that is prime. */

#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* indices below the offset are undefined */

    seq_t nn = (seq_t)n;           /* avoid mixed‑type multiplication */
    uint64_t q = 2;                /* first prime candidate for q */

    for (;; ++q) {
        if (!is_prime_small(q))   /* skip non‑primes */
            continue;

        /* Compute p = n*(q-1)+1.  Use seq_t arithmetic so the result grows
           with the required precision. */
        seq_t p = nn * (seq_t)(q - 1ULL) + 1;

        if (is_prime_big(p))     /* first prime p that satisfies the
                                   relation is the answer */
            return p;
    }
}
