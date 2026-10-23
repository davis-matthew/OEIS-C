/*
 * A023240  Primes p such that 10*p + 9 is also prime.
 * https://oeis.org/A023240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1879, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Prime p such that 10p + 9 is also prime. 
   Computes terms by scanning primes and testing the condition,
   caching results for subsequent calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* static cache of found terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t last_checked = 1;   /* last integer examined */

/* enlarge cache if necessary */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0) abort();          /* out‑of‑memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* fill cache up to target index (1‑based) */
static void fill_cache(idx_t target)
{
    if (cache_len >= (size_t)target) return;

    while (cache_len < (size_t)target) {
        /* find next prime p */
        do {
            ++last_checked;
        } while (!is_prime(last_checked));

        uint64_t p = last_checked;
        uint64_t q = 10 * p + 9ULL;

        if (is_prime(q)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)p;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* offset 1, harmless fallback */
    fill_cache(n);
    return cache[n - 1];
}
