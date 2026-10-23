/*
 * A141967  Primes congruent to 5 mod 28.
 * https://oeis.org/A141967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th prime that is congruent to 5 (mod 28)
 * Range:     u8 n <= 5, u16 n <= 547, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"

/* Compute the n-th prime ≡ 5 (mod 28).  Caches results so that calls are
   fast in any order. */
seq_t a(idx_t n)
{
    /* A dynamic cache of found terms.  cache_len holds the number of
       stored terms, cache_cap the allocated capacity.  The cache is
       grown as needed. */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;

    /* The current candidate ≡ 5 (mod 28).  We use a 128‑bit integer
       to avoid overflow for any realistic n. */
    static unsigned __int128 cand = 5;

    /* Generate terms until we have at least n of them. */
    while ((idx_t)cache_len < n)
    {
        /* Only consider values that fit in seq_t.  If the value is
           larger than can be represented we simply skip it – the
           sequence could not be expressed in seq_t anyway. */
        if ((sizeof(seq_t) >= sizeof(unsigned __int128)) ||
            ((unsigned __int128) (unsigned long long)cand == cand))
        {
            /* Convert to uint64_t for the small primality test.  For
               types larger than 64 bits we use the big test. */
            uint64_t cand_u64 = (uint64_t)cand;
            bool prime = (sizeof(seq_t) <= sizeof(uint64_t))
                         ? is_prime_small(cand_u64)
                         : is_prime_big((seq_t)cand_u64);

            if (prime)
            {
                /* Grow the cache if necessary. */
                if (cache_len == cache_cap)
                {
                    size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp)  /* allocation failure: propagate undefined */
                        abort();
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)cand_u64;
            }
        }
        /* Move to the next number ≡ 5 (mod 28). */
        cand += 28;
    }
    return cache[n - 1];
}
