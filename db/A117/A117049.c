/*
 * A117049  Primes of the form 22*(n^2)+1.
 * https://oeis.org/A117049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 24, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function yields the nth prime of the form 22*k^2+1
   by searching successive integers k.  The sequence is not
   given by a closed formula; instead we generate primes on
   demand, caching the results for subsequent calls. */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;   /* array of found primes */
static idx_t  cached_count = 0;   /* how many primes we have cached */
static idx_t  k_next = 1;       /* next k to test */
static size_t cache_capacity = 0;

static void ensure_cache_capacity(idx_t min_needed)
{
    if (cache_capacity >= (size_t)min_needed)
        return;
    size_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
    while (new_cap < (size_t)min_needed)
        new_cap *= 2;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (new_cache == NULL) {
        /* In a production environment you might handle the error differently.
           For this exercise we simply abort. */
        abort();
    }
    cache = new_cache;
    cache_capacity = new_cap;
}

static void extend_sequence_to(idx_t target)
{
    while (cached_count < target) {
        seq_t val;
        /* find the next k such that 22*k^2+1 is prime */
        for (;;) {
            val = (seq_t)22 * (seq_t)k_next * (seq_t)k_next + 1;
            if (is_prime_big(val))
                break;
            ++k_next;
        }
        /* store the found prime */
        ensure_cache_capacity(cached_count + 1);
        cache[cached_count++] = val;
        ++k_next;   /* move to the next candidate for future searches */
    }
}

seq_t a(idx_t n)
{
    if (n < 1)   /* the sequence is only defined for n >= 1 */
        return 0;

    if (cached_count < n)
        extend_sequence_to(n);

    return cache[n - 1];
}
