/*
 * A142412  Primes congruent to 43 mod 48.
 * https://oeis.org/A142412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 411, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A142412: primes congruent to 43 mod 48.
   The sequence is indexed from 1; a(1) = 43. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

/* Return the n‑th prime congruent to 43 (mod 48).  This implementation
   builds the sequence incrementally and caches all found terms so that
   calls in any order are efficient. */
seq_t a(idx_t n)
{
    /* Cache of previously found primes */
    static seq_t *prime_cache = NULL;   /* array of primes */
    static size_t prime_count = 0;      /* number of cached primes   */
    static idx_t next_candidate = 43;   /* next integer ≡ 43 (mod 48) */

    /* The sequence is defined for n ≥ 1.  For n = 0 we simply return 0
       as a safe value; the caller must not request an offset less than 1. */
    if (n == 0)
        return 0;

    /* Grow the cache until it contains at least n entries. */
    while (prime_count < n)
    {
        /* Test the current candidate.  It is guaranteed to be  ≡ 43 (mod 48). */
        if (is_prime_small((seq_t)next_candidate))
        {
            /* Allocate a larger buffer.  Use a C style cast because the
               type seq_t may be a built‑in integer type or a trivial type
               in the C case, and C++ allows a cast from void* to any
               object pointer.  If seq_t is a non‑trivial type the build
               is expected to provide a proper allocator. */
            seq_t *tmp = (seq_t *)realloc(prime_cache,
                                          (prime_count + 1) * sizeof(seq_t));
            if (tmp == NULL)
                abort();                     /* out of memory */

            prime_cache = tmp;
            prime_cache[prime_count++] = (seq_t)next_candidate;
        }
        /* Advance to the next number in the arithmetic progression 48k+43. */
        next_candidate += 48;
    }

    return prime_cache[n - 1];
}
