/*
 * A142027  Primes congruent to 23 mod 31.
 * https://oeis.org/A142027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 219, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142027 – primes congruent to 23 (mod 31) */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* provides: bool is_prime(idx_t). */

/* keep a cache of found primes; last_checked holds the largest tested number */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t last_checked = 22;   /* start from 23 on first increment */

/* grow the cache array when needed */
static void ensure_cache_capacity(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* value a(n) = nth prime ≡ 23 (mod 31), n ≥ 1 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for index 0 */

    /* generate candidates until we have at least n terms */
    while (cache_len < n) {
        last_checked++;                     /* next candidate */
        if (last_checked % 31ULL == 23ULL && is_prime(last_checked)) {
            ensure_cache_capacity();
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
