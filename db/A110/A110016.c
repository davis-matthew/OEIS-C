/*
 * A110016  Numbers n such that (n^2-7)/6 is prime.
 * https://oeis.org/A110016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n-th odd number m such that (m^2-7)/6 is prime
 * Range:     u8 n <= 3, u16 n <= 36, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function directly generates terms on demand without any cached array. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(uint64_t) */

/* a(n) returns the n‑th term of OEIS A110016;  n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) abort();          /* sequence is 1‑indexed; input <1 is invalid */

    idx_t   count     = 0;        /* how many valid terms found so far */
    seq_t   candidate = 5;        /* first odd number that can satisfy the condition */

    while (count < n)
    {
        /* Compute p = (candidate^2 - 7)/6; the division is exact for odd
           candidate ≡ 1 or 5 (mod 6). */
        seq_t sq = candidate * candidate;
        seq_t p  = (sq - 7) / 6;

        /* Test primality.  p is small enough to fit in uint64_t for all
           indices we are expected to handle. */
        if (is_prime((uint64_t)p))
        {
            count++;
            if (count == n) return candidate;
        }

        /* Advance to the next odd ≡ 1 or 5 (mod 6). */
        if (candidate % 6 == 5)
            candidate += 2;   /* 5 → 7, 11 → 13, … */
        else
            candidate += 4;   /* 1 → 5, 7 → 11, … */
    }

    /* This point is unreachable, but arm the compiler. */
    abort();
}
