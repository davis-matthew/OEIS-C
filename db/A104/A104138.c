/*
 * A104138  Smallest prime followed by n or more composites.
 * https://oeis.org/A104138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest prime that is followed by at least n
 *            composite numbers. For n = 0, the definition is a(0) = 2. For n
 *            > 0, a(n) is the first prime p for which p < q = next prime and
 *            (q - p - 1) ≥ n
 * Range:     u8 n <= 13, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Essentital utilities */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* is_prime(uint64_t) is provided by the build’s utils library. */
#include "../utils/is_prime.h"

/* --------------------------------------------------------------------- */
/* Helper to find the next prime after a given one.                       */
/* --------------------------------------------------------------------- */
static seq_t next_prime_after(seq_t current)
{
    uint64_t cand = (uint64_t)current + 1;
    if ((cand & 1ULL) == 0ULL)      /* start with an odd number */
        cand++;
    while (!is_prime(cand))
        cand += 2ULL;
    return (seq_t)cand;
}

/* --------------------------------------------------------------------- */
/* Main sequence function.                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* a(0) is defined as the very first prime. */
    if (n == 0)
        return (seq_t)2;

    /* Start searching from the prime 2. */
    seq_t p = (seq_t)2;
    for (;;) {
        seq_t q = next_prime_after(p);                 /* next prime after p   */
        seq_t gap = q - p - (seq_t)1;                  /* composites following p */

        if (gap >= (seq_t)n)                          /* condition satisfied   */
            return p;                                 /* smallest prime found */

        p = q;                                        /* advance to next prime */
    }
}
