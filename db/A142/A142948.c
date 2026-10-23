/*
 * A142948  Primes congruent to 51 mod 64.
 * https://oeis.org/A142948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 176, u32 n <= 194, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the primes that are congruent to 51 modulo 64.
   We iterate over the numbers 51 + 64·k, test each for primality,
   and keep the found primes in a static cache. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t) */
#include "../utils/grow_seq.h"       /* grow_seq(seq_t **, size_t *, size_t) */

seq_t a(idx_t n)
{
    /* static cache for primes found so far */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t count = 0;
    static uint64_t next_candidate = 51;   /* 51 + 64·k */

    if (n < 1)
        return 0;   /* undefined for n <= 0, but caller will not do this */

    /* Grow the cache until it contains at least n elements. */
    while (count < (size_t)n)
    {
        /* Test primality using the provided routine. */
        if (is_prime_big((seq_t)next_candidate))
        {
            /* Ensure there is space for the new element. */
            if (count >= cap)
                grow_seq(&cache, &cap, count + 1);

            cache[count] = (seq_t)next_candidate;  /* store the prime */
            ++count;
        }

        next_candidate += 64;   /* move to the next number in the progression */
    }

    /* Return the requested term (1‑based index). */
    return cache[(size_t)n - 1];
}
