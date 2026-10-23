/*
 * A103506  Smallest prime p such that 2n+1 = 2q + p for some odd prime q, or
 *          0 if no such prime exists.
 * https://oeis.org/A103506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 784, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A103506: Smallest prime p such that 2n+1 = 2q + p
   for some odd prime q, or 0 if no such prime exists. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* OEIS offset starts at 1.  For n < 4 the definition gives 0. */
    if (n < 4U)
        return (seq_t)0;

    /* Compute 2n+1, which fits in 64 bits for the intended use range. */
    uint64_t s = 2ULL * n + 1ULL;

    /* Search for the smallest odd prime p ≤ s-6 (s-6 ≥ 3 when n≥4). */
    for (uint64_t p = 3ULL; p <= s - 6ULL; p += 2ULL)
    {
        if (!is_prime(p))
            continue;                      /* skip non‑primes */

        uint64_t q = (s - p) >> 1U;        /* q = (2n+1 - p)/2 */

        if (q < 3ULL)
            continue;                      /* q must be an odd prime ≥3 */

        if (is_prime(q))
            return (seq_t)p;               /* smallest p found */
    }

    /* No suitable prime was found (happens only for n < 4). */
    return (seq_t)0;
}
