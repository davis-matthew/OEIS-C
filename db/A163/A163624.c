/*
 * A163624  Numbers k such that 120*k+1 is prime.
 * https://oeis.org/A163624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A163624: numbers k such that 120*k+1 is prime.
   The function returns a(n), the n‑th such k (1‑based indexing). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"    /* provides bool is_prime(idx_t) */

seq_t a(idx_t n)
{
    /* Static state for memoised generation */
    static seq_t *cache = NULL;      /* array of found k values              */
    static size_t cache_len = 0;     /* number of stored values              */
    static size_t cache_cap = 0;     /* current capacity of the array        */
    static idx_t last_k = 0;         /* last candidate k that was examined   */

    /* Ensure cache can hold at least n values, generating new ones as needed */
    while (cache_len < (size_t)n) {
        /* Next candidate k */
        ++last_k;

        /* Test whether 120*k + 1 is prime.  Use 64‑bit arithmetic, safe
           for all k that occur in the tested range (up to a few millions). */
        uint64_t cand = 120ULL * (uint64_t)last_k + 1ULL;
        if (is_prime((idx_t)cand)) {
            /* Grow storage if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
#ifdef __cplusplus
                /* In C++ (arbitrary‑precision seq_t) use new[] and copy */
                seq_t *new_cache = new seq_t[new_cap];
                for (size_t i = 0; i < cache_len; ++i)
                    new_cache[i] = cache[i];
                delete[] cache;
                cache = new_cache;
#else
                /* In C (unsigned integer seq_t) use realloc */
                void *p = realloc(cache, new_cap * sizeof(seq_t));
                cache = (seq_t*)p;   /* cast to seq_t* is safe in C */
#endif
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_k;
        }
    }

    /* Return the requested term */
    return cache[n - 1];
}
