/*
 * A229854  Primes of the form 384*k + 1.
 * https://oeis.org/A229854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 34, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th prime of the form 384*k + 1.
   The implementation keeps a growing cache of found primes so that
   calls with arbitrary n (not necessarily monotone) are efficient. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Realloc helper that works in both C and C++.
   In C++ we use static_cast, in C we use a C-style cast. */
#if defined(__cplusplus)
    #define REALLOC(ptr, sz) static_cast<decltype(ptr)>(realloc((ptr), (sz)))
#else
    #define REALLOC(ptr, sz) ((seq_t *)realloc((ptr), (sz)))
#endif

/* Static cache of found primes, indexed from 0. */
static seq_t *cache      = NULL;   /* array of found primes   */
static size_t cache_size = 0;      /* number of primes cached */
static size_t cache_cap  = 0;      /* allocated capacity      */
/* Next k to test (candidate p = 384*k + 1).  We start at k = 2 because
   384*1+1 = 385 is not prime. */
static idx_t next_k = 2;

/* Return the n‑th prime of the form 384*k + 1 (a(1) is the first). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n < 1 */

    /* Grow the cache until it contains at least n primes. */
    while ((idx_t)cache_size < n)
    {
        /* Compute the candidate prime = 384*next_k + 1 using 64‑bit arithmetic. */
        seq_t cand = (seq_t)(((uint64_t)384 * next_k) + 1ULL);

        /* Test primality of cand. */
        if (is_prime_big(cand))
        {
            /* Ensure capacity of the cache. */
            if (cache_size == cache_cap)
            {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = REALLOC(cache, cache_cap * sizeof(seq_t));
                if (!tmp)
                    return 0;   /* allocation failed */
                cache = tmp;
            }
            /* Store the prime. */
            cache[cache_size++] = cand;
        }

        ++next_k;   /* next candidate */
    }

    /* Return the n-th prime (1‑based index). */
    return cache[n - 1];
}
