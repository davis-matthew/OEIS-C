/*
 * A214588  Primes p such that p mod 16 < 8.
 * https://oeis.org/A214588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 3284, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th prime p such that p mod 16 < 8
   (equivalently p XOR 8 = p + 8).  It builds a static cache of primes
   satisfying the condition and expands it lazily as needed. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/seq_realloc.h"   /* assumes seq_realloc() is declared */

seq_t a(idx_t n)
{
    /* static cache of primes that satisfy p mod 16 < 8 */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* current capacity of cache */
    static size_t cache_size = 0;  /* number of primes stored */

    /* next integer to test for primality */
    static idx_t next_candidate = 2;

    /* grow the cache until it contains at least n primes */
    while (cache_size < n)
    {
        seq_t cand = (seq_t)next_candidate;
        bool is_p;

        /* trivial checks for candidate */
        if (next_candidate == 2) {
            is_p = true;              /* 2 is prime */
        } else if ((next_candidate & 1ULL) == 0) {
            is_p = false;             /* even numbers > 2 are composite */
        } else {
            is_p = isPrimeGeneric(cand);
        }

        /* store if it is prime and satisfies the condition */
        if (is_p && ((next_candidate & 0xFULL) < 8ULL))
        {
            if (cache_size == cache_cap)   /* grow cache if needed */
            {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_size++] = cand;
        }

        ++next_candidate;   /* advance to the next integer */
    }

    return cache[n - 1];
}
