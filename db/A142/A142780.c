/*
 * A142780  Primes congruent to 53 mod 59.
 * https://oeis.org/A142780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 53 (mod 59)
 * Range:     u8 n <= 1, u16 n <= 109, u32 n <= 69, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   free_seq_array, is_prime_big, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A142780)  Offset: 1 */

#include <stdbool.h>
#include <stddef.h>
#include <string.h>            /* for memcpy */

#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/free_seq_array.h"

/* static cache of found primes */
static seq_t *cache = NULL;      /* array of primes */
static size_t cache_size = 0;    /* number of primes stored */
static size_t cache_cap   = 0;    /* allocated capacity       */
static seq_t  candidate   = 53;   /* next number ≡ 53 (mod 59) */
static bool   initialized = false;

static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    /* grow to at least needed, doubling until enough */
    size_t new_cap = cache_cap > 0 ? cache_cap : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = seq_alloc_array(new_cap);
    if (!tmp) {
        /* allocation failure – terminate safely */
        cache_size = 0;
        cache_cap  = 0;
        cache = NULL;
        return;
    }
    /* copy existing primes */
    if (cache && cache_size)
        memcpy(tmp, cache, cache_size * sizeof(seq_t));
    /* free old array */
    if (cache)
        free_seq_array(cache);
    cache     = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for offset 1, but guard anyway */

    if (!initialized) {
        /* initial allocation – enough for many primes (≈200 000) */
        cache_cap = 20000;
        cache     = seq_alloc_array(cache_cap);
        cache_size = 0;
        candidate  = 53;      /* start from the first possible prime */
        initialized = true;
    }

    /* grow cache if required */
    ensure_capacity(n);

    /* extend cache until it contains the nth prime */
    while (cache_size < (size_t)n) {
        if (is_prime_big(candidate)) {
            cache[cache_size++] = candidate;
        }
        candidate += 59;        /* next number ≡53 (mod 59) */
    }

    return cache[(size_t)n - 1];
}
