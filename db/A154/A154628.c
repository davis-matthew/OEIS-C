/*
 * A154628  Primes congruent to 35 mod 73.
 * https://oeis.org/A154628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 83, u32 n <= 48, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes congruent to 35 mod 73. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static cache of primes congruent to 35 mod 73. */
    static seq_t *cache = NULL;        /* array of primes */
    static idx_t  count = 0;           /* number of cached primes */
    static size_t capacity = 0;        /* allocated array size */
    static seq_t  next_candidate = 181;/* first prime in the progression */

    if (n == (idx_t)0) return (seq_t)0; /* undefined but safe */

    /* Generate primes until we have at least n of them. */
    while (count < n)
    {
        /* Test whether the current candidate is prime. */
        if (is_prime_big(next_candidate))
        {
            /* Grow the cache if necessary. */
            if (count >= capacity)
            {
                size_t new_cap = capacity ? capacity * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);      /* allocation failure */
                cache = tmp;
                capacity = new_cap;
            }
            cache[count++] = next_candidate;
        }

        /* Advance to the next odd number congruent to 35 mod 73. */
        next_candidate += 146; /* 73*2 ensures the parity stays odd */
    }

    return cache[n - 1];
}
