/*
 * A030432  Primes of form 10n+7.
 * https://oeis.org/A030432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1239, u32 n <= 1442, u64 n <= 8944,
 *            u128 n <= 8944, big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime of the form 10k+7.
   The sequence is enumerated for n >= 1. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;           /* array of discovered primes */
static idx_t cache_count = 0;         /* number of primes stored in cache */
static idx_t cache_cap = 0;           /* current allocation size of cache */
static idx_t next_k = 1;              /* k for the next candidate 10*k+7 */

/* Return the n-th prime congruent to 7 mod 10. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* Offset does not matter for n < 1 */

    /* Initialisation on first call. */
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)7;          /* a(1) = 7 */
        cache_count = 1;
        next_k = 1;                   /* next candidate is k = 1 -> 17 */
    }

    /* Grow the cache to contain at least n primes. */
    while (cache_count < n) {
        /* Candidate of the form 10*k + 7. */
        seq_t cand = (seq_t)10 * (seq_t)next_k + (seq_t)7;

        if (is_prime_big(cand))
            cache[cache_count++] = cand;

        next_k++;

        /* Resize storage if necessary. */
        if (cache_count >= cache_cap) {
            ensure_capacity(cache_count + 1, &cache_cap,
                            (void **)&cache, sizeof(seq_t));
        }
    }

    return cache[n - 1];
}
