/*
 * A155737  Primes of the form 4*n^2 + 2*n -1.
 * https://oeis.org/A155737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 56, u32 n <= 41, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A155737 – primes of the form 4*n^2 + 2*n – 1.
   The n‑th term is the n‑th prime that can be expressed by this
   quadratic.  Candidates are generated sequentially and tested with
   is_prime_big().  Results are cached so that repeated calls to any
   n are fast. */
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;      /* array of found primes          */
static size_t cache_count = 0;   /* number of primes stored        */
static size_t cache_cap   = 0;   /* capacity of the array          */
static idx_t  last_k      = 0;   /* last candidate k processed     */

/* Grow the cache array to at least new_cap elements.
   aborts on allocation failure by resetting the cache. */
static void
grow_cache(size_t new_cap)
{
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        free(cache);
        cache = NULL;
        cache_count = 0;
        cache_cap   = 0;
        last_k      = 0;
        return;
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t
a(idx_t n)
{
    /* Note: OEIS offset is 1; a(1) is the first prime of this form. */
    if (n == 0u)
        return 0;                     /* undefined for non‑positive indices */

    /* If the value is already cached, return it. */
    if ((size_t)n <= cache_count)
        return cache[(size_t)n - 1];

    /* Ensure sufficient capacity for the desired index. */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < (size_t)n)
            new_cap = (size_t)n;
        grow_cache(new_cap);
        if (cache_cap == 0)          /* allocation failed */
            return 0;
    }

    /* Generate candidates until the n‑th prime of this form is found. */
    for (idx_t k = last_k + 1; ; ++k) {
        seq_t k_seq = (seq_t)k;
        seq_t val   = (4 * k_seq * k_seq) + (2 * k_seq) - 1;

        if (is_prime_big(val)) {
            cache[cache_count++] = val;

            if ((size_t)cache_count == n) {
                last_k = k;          /* remember last tested k          */
                return val;
            }
        }
    }
}
