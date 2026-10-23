/*
 * A135657  Nonprimes of the form 24n+7.
 * https://oeis.org/A135657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Non-primes of the form 24n + 7 (OEIS A135657) */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* offset starts at 1 */

    /* static cache of all terms found so far */
    static seq_t *cache      = NULL;/* array of terms */
    static idx_t cache_len   = 0;   /* number of cached terms */
    static idx_t cache_cap   = 0;   /* current capacity of cache */
    static idx_t last_k      = 1;   /* next k to test (candidate = 24*k+7) */

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        /* candidate as a seq_t value */
        seq_t cand = ((seq_t)24 * last_k) + 7;
        last_k++;

        /* is_prime takes an idx_t; cand fits in 64 bits for all required n
           (the 10000th term is far below 2^64). */
        if (cand >= 2 && !is_prime((idx_t)cand)) {
            if (cache_len == cache_cap) {           /* grow cache */
                idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();                  /* out of memory */
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_len++] = cand;                /* store term */
        }
    }

    return cache[n - 1];
}
