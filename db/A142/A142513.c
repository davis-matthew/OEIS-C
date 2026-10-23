/*
 * A142513  Primes congruent to 11 mod 52.
 * https://oeis.org/A142513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 283, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 11 (mod 52).  
   Computed by testing successive numbers 11 + 52·k and caching
   the primes found. */

#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t *cache      = NULL;   /* dynamic array of found primes         */
static size_t cache_size = 0;      /* number of stored primes                */
static size_t cache_cap  = 0;      /* allocated capacity of cache            */
static idx_t next_candidate = 11;  /* next number to test, ≡ 11 (mod 52)     */

seq_t a(idx_t n)
{
    if (n == 0u) return 0;  /* offset 1; callers will not use n = 0 */

    /* Grow the cache until it contains at least n primes. */
    while (cache_size < n)
    {
        /* Ensure there is space for one more prime. */
        if (cache_size == cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();          /* out‑of‑memory: terminate */
            cache   = tmp;
            cache_cap = new_cap;
        }

        if (is_prime(next_candidate))
            cache[cache_size++] = (seq_t)next_candidate;

        next_candidate += 52;          /* next candidate ≡ 11 (mod 52) */
    }

    return cache[n - 1];
}
