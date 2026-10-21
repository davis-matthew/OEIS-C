/*
 * A089376  Primes of the form k^2 - 7*k + 7.
 * https://oeis.org/A089376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 50, u32 n <= 40, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = value of the n-th prime of the form k^2 - 7*k + 7,
 * computed by incremental search.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;      /* array of found primes */
static size_t cache_cap = 0;     /* allocated capacity of cache   */
static size_t cache_len = 0;     /* number of primes stored       */
static idx_t next_k = 0;         /* next k to test                */

/* Ensure cache has at least `needed` slots */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 128;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        return; /* allocation failure – leave cache unchanged */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th prime of the form k^2 - 7*k + 7 (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0)           /* undefined; caller should not use */
        return (seq_t)0;

    /* lazy initialise the cache with the first occurrence (k = 0) */
    if (cache_len == 0) {
        ensure_cache_cap(1);
        cache[0] = (seq_t)7;    /* k = 0 gives 7 */
        cache_len = 1;
        next_k   = 1;           /* start testing k = 1 */
    }

    /* generate primes until we have at least n of them */
    while (cache_len < (size_t)n) {
        seq_t k   = (seq_t)next_k;
        seq_t val = k * k - (seq_t)7 * k + (seq_t)7;

        if (val >= (seq_t)2 && is_prime_big(val)) {
            /* avoid duplicate 7 at k = 7 */
            if (cache_len == 0 || val != cache[cache_len - 1]) {
                ensure_cache_cap(cache_len + 1);
                cache[cache_len++] = val;
            }
        }
        next_k++;
    }
    return cache[n - 1];
}
