/*
 * A142418  Primes congruent to 5 mod 49.
 * https://oeis.org/A142418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 5 (mod 49)
 * Range:     u8 n <= 2, u16 n <= 161, u32 n <= 112, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, seq_alloc_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps a static dynamic array of the primes
   found so far.  For each call it extends the array until it
   contains at least n elements.  The sequence index n is 1‑based. */

/* Standard headers */
#include <stdlib.h>
#include <stdbool.h>

/* Utility functions provided by the project */
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_realloc.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* index 0 is undefined; return 0 */

    /* Static cache of found primes */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_cnt  = 0;
    static idx_t  last_k     = 0;  /* next candidate index (k)   */

    /* Extend the cache until it contains at least n primes */
    while (cache_cnt < n)
    {
        /* Candidate of the form 49*k + 5 */
        idx_t        k   = last_k;
        seq_t        cand = (seq_t)((idx_t)k * 49 + 5);

        /* If it is prime, store it in the cache */
        if (is_prime_big(cand))
        {
            if (cache_cnt == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_cnt++] = cand;
        }

        last_k = k + 1;   /* advance to next candidate */
    }

    return cache[n - 1];   /* n is 1‑based */
}
