/*
 * A142851  Primes congruent to 53 mod 61.
 * https://oeis.org/A142851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 114, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes ≡ 53 (mod 61) */
/* Computes a(n) as the n‑th prime in that residue class. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* stored primes */
    static size_t cap    = 0;       /* allocated capacity */
    static size_t len    = 0;       /* number of primes stored */
    static idx_t  next_candidate = 53;   /* first candidate 61·0 + 53 */

    /* Grow the cache until it holds at least n primes. */
    while (len < n)
    {
        /* Skip non‑primes in this residue class. */
        while (!is_prime(next_candidate))
            next_candidate += 61;

        /* Ensure there is room for the new prime. */
        if (len + 1 > cap)
        {
            size_t new_cap = cap ? cap * 2 : 16;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();          /* out‑of‑memory */
            cache = tmp;
            cap   = new_cap;
        }

        /* Store the found prime and advance to the next candidate. */
        cache[len++] = (seq_t)next_candidate;
        next_candidate += 61;
    }

    return cache[n - 1];
}
