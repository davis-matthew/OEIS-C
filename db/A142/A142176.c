/*
 * A142176  Primes congruent to 29 mod 39.
 * https://oeis.org/A142176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime numbers congruent to 29 mod 39
 * Range:     u8 n <= 2, u16 n <= 273, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;          /* storage for computed primes   */
static size_t  cache_len  = 0;       /* number of primes stored       */
static size_t  cache_cap  = 0;       /* current capacity of cache     */
static uint64_t next_candidate = 29; /* next number to test (≡29 mod 39) */

static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache   = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* n is 1‑based; n == 0 returns 0 (undefined for the OEIS sequence) */
    if (n == 0) return 0;
    while (cache_len < (size_t)n) {
        if (is_prime((idx_t)next_candidate)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)next_candidate;
        }
        next_candidate += 39;
    }
    return cache[n - 1];
}
