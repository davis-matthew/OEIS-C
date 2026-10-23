/*
 * A144918  Duplicate of A007528.
 * https://oeis.org/A144918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) is the n‑th prime congruent to 5 modulo 6 (primes of the
 *            form 6k-1)
 * Range:     i8 n <= 15, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The value is computed by searching primes of that form and
   testing each candidate with the provided is_prime_big routine. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache      = NULL;   /* cached terms */
static size_t cache_cap  = 0;      /* cache capacity */
static size_t cache_len  = 0;      /* number of cached terms */
static seq_t last_candidate = 0;   /* last generated 6k-1 prime */

/* Ensure that at least n terms are cached. */
static void ensure_terms(idx_t n)
{
    if (n <= cache_len)
        return;

    /* Generate terms until we have at least n. */
    while (cache_len < n) {
        seq_t cand;
        if (cache_len == 0) {
            cand = 5;                      /* first 6k-1 prime */
        } else {
            cand = last_candidate + (seq_t)6;
        }

        /* Skip non‑primes; only numbers of the form 6k-1 are tested. */
        while (!is_prime_big(cand))
            cand += (seq_t)6;

        /* Grow the cache if necessary. */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 32;
            seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache)          /* allocation failure – abort */
                return;
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* Store the found prime. */
        cache[cache_len++] = cand;
        last_candidate = cand;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)          /* not defined for n=0 – return 0 */
        return 0;
    ensure_terms(n);
    return cache[n - 1];
}
