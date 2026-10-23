/*
 * A142919  Primes congruent to 53 mod 63.
 * https://oeis.org/A142919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 185, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of primes congruent to 53 (mod 63): a(n) is the nth prime p with
   p % 63 == 53, offset 1. This implementation searches numbers
   of the form 53 + 63*k and tests them for primality. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"  /* provides: bool is_prime(idx_t) */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* primes found so far */
    static size_t cap = 0;               /* capacity of cache */
    static size_t count = 0;             /* number of primes stored */
    static idx_t next_candidate = 53;    /* next number ≡53 (mod 63) */

    /* Generate primes until we have at least n elements */
    while (count < (size_t)n)
    {
        if (is_prime(next_candidate))
        {
            /* Grow cache if needed */
            if (count == cap)
            {
                size_t new_cap = cap ? cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();                 /* out‑of‑memory */
                cache = tmp;
                cap = new_cap;
            }
            /* Store the prime (truncated to seq_t if necessary) */
            cache[count++] = (seq_t)next_candidate;
        }
        /* Advance to the next candidate of the form 53 + 63*k */
        next_candidate += 63;
    }
    /* Return the requested term (offset 1) */
    return cache[n - 1];
}
