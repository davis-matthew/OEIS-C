/*
 * A099007  Primes of the form 6n^2 - 2n - 1.
 * https://oeis.org/A099007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 35, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: produce the n‑th prime of the form 6k²‑2k‑1 by
 * incrementally testing successive values of k and caching all found
 * primes. */

#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_big.h"

/* Global static cache for memoisation across calls. */
static seq_t *cache = NULL;     /* array of primes found so far   */
static size_t cache_cap = 0;    /* allocated capacity of 'cache'  */
static size_t cache_len = 0;    /* number of primes stored        */
static idx_t next_k = 1;        /* next k to test in 6k²‑2k‑1     */

seq_t a(idx_t n)
{
    /* Keep generating candidates until we have at least n primes. */
    while (cache_len < n) {
        /* Compute candidate value 6k² – 2k – 1 using seq_t arithmetic. */
        seq_t v = (seq_t)6 * (seq_t)next_k * (seq_t)next_k
                  - (seq_t)2 * (seq_t)next_k
                  - (seq_t)1;

        /* Test primality. is_prime_big works for all seq_t sizes. */
        if (is_prime_big(v)) {
            /* Grow cache if needed. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
                if (!tmp) abort();               /* OOM – abort */
                memcpy(tmp, cache, cache_len * sizeof(seq_t));
                free(cache);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = v;    /* store prime */
        }
        ++next_k;                     /* next k to examine */
    }

    return cache[n - 1];              /* 1‑based indexing */
}
