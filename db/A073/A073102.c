/*
 * A073102  Primes of the form 210n + 1.
 * https://oeis.org/A073102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes the n-th prime of the form 210*k + 1, with the sequence offset 1.
   i.e. a(n) is the n-th prime p such that p % 210 == 1.
   The function uses a simple incremental search and caches found terms
   so that calls with arbitrary order of n are efficient.
*/
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrime.h"   /* provides: bool isPrime(idx_t x); */

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;          /* undefined for offset < 1 */

    /* Static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static idx_t  next_k     = 1;   /* next k to test in the 210*k+1 progression */

    /* Generate and cache terms until we have at least n of them */
    while (cache_len < (size_t)n)
    {
        /* Next candidate: 210*next_k + 1 */
        uint64_t cand64 = 210ull * (uint64_t)next_k + 1ull;

        if (isPrime((idx_t)cand64))
        {
            /* Grow cache if necessary */
            if (cache_len >= cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                /* realloc returning void* must be cast for both C and C++ */
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)   /* allocation failed – return zero as graceful failure */
                    return (seq_t)0;
                cache     = tmp;
                cache_cap = new_cap;
            }
            /* Store the discovered prime */
            cache[cache_len] = (seq_t)cand64;
            ++cache_len;
        }
        ++next_k;   /* advance to the next candidate */
    }

    return cache[(size_t)(n - 1)];   /* a(1) corresponds to cache[0] */
}
