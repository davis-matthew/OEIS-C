/*
 * A077359  Primes whose external digits form a prime. Or primes from which
 *          deleting the internal digits leaves a prime.
 * https://oeis.org/A077359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A077359: primes whose first and last digit form a prime
   (including all single‑digit primes).  The function builds a cache
   of qualifying primes and answers queries in constant amortised time. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_small.h"

/* Helper: most significant digit of a seq_t value.  It works
   for all unsigned seq_t types. */
static seq_t msd(seq_t v)
{
    while (v >= 10)
        v /= 10;
    return v;
}

/* Sequence accessor.  n >= 1. */
seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* array of qualifying primes   */
    static size_t cache_cap  = 0;      /* current allocation capacity */
    static size_t cache_len  = 0;      /* number of stored terms       */
    static idx_t  candidate  = 1;      /* next integer to test          */
    static bool   init       = false;  /* lazy initialisation flag      */

    if (!init) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        init = true;
    }

    /* Generate primes until the cache contains at least n terms. */
    while (cache_len < n) {
        /* Move to the next integer.  candidate starts at 1, so the first
           tested integer is 2. */
        candidate++;

        /* The candidate must be prime. */
        if (!is_prime(candidate))
            continue;

        /* Determine if the primality condition on the external digits
           is satisfied.  For single‑digit primes it holds automatically.
           For multi‑digit primes we form the two‑digit number
           10*first_digit + last_digit and test it with is_prime_small. */
        bool qualifies;
        if (candidate < 10) {
            qualifies = true;
        } else {
            seq_t first = msd((seq_t)candidate);
            seq_t last  = (seq_t)(candidate % 10);
            seq_t ext   = first * 10 + last;
            qualifies = is_prime_small(ext);
        }

        if (qualifies) {
            /* Grow the cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    free(cache);
                    return 0;   /* Allocation failure – caller will abort */
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)candidate;
        }
    }

    return cache[n - 1];
}
