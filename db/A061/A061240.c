/*
 * A061240  Prime numbers == 5 (mod 9).
 * https://oeis.org/A061240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the nth prime congruent to 5 (mod 9), found by,
   scanning candidates of the form 9*k+5 and testing them for primality. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* static cache of already computed terms */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_k = 0;          /* next k to try (candidate = 9*k+5) */

/* Ensure there are at least n terms stored in cache. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;                 /* already have enough terms */

    if (cache == NULL) {                       /* first time: allocate */
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_len < n) {
        idx_t candidate = 9ull * next_k + 5ull;

        /* Find the next prime ≡ 5 (mod 9). */
        while (!is_prime(candidate)) {
            ++next_k;
            candidate = 9ull * next_k + 5ull;
        }

        cache[cache_len++] = (seq_t)candidate;  /* store the prime             */
        ++next_k;                               /* advance to next candidate   */

        if (cache_len >= cache_cap) {           /* grow cache if necessary     */
            cache_cap <<= 1;                    /* double capacity             */
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }
    }
}

/* Return the nth element of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n < 1, return 0 */
    ensure_cache(n);
    return cache[n - 1];
}
