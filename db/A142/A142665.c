/*
 * A142665  Primes congruent to 1 mod 57.
 * https://oeis.org/A142665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 185, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes congruent to 1 modulo 57 (search/sieve) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Cached values of the sequence */
static seq_t *cache = NULL;          /* sequence values */
static idx_t   cache_size = 0;       /* number of terms computed */
static idx_t   cache_cap   = 0;       /* allocated capacity */
static idx_t   next_k      = 3;       /* last k processed (57*k+1) */

/* Return the n‑th prime p with p ≡ 1 (mod 57), a(1) = 229. */
seq_t a(idx_t n)
{
    if (n == 0)                /* offset: a(0) is undefined, return 0 */
        return (seq_t)0;

    /* Allocate or grow the cache to hold at least n entries */
    if (cache == NULL) {
        cache_cap = (n > (idx_t)16) ? n : (idx_t)16;
        cache = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;   /* allocation failed */
        cache_size = 0;
    } else if (cache_size < n && cache_cap < n) {
        cache_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)cache_cap * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;
        cache = tmp;
    }

    /* Generate terms until we reach the desired index */
    while (cache_size < n) {
        next_k++;                            /* next candidate k */
        idx_t cand = 57 * next_k + 1;        /* 57*k + 1 */

        if (is_prime(cand)) {
            cache[cache_size++] = (seq_t)cand;
        }
    }

    return cache[n - 1];                     /* zero‑based index */
}
