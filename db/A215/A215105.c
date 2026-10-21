/*
 * A215105  Primes congruent to {2, 3, 5} mod 19.
 * https://oeis.org/A215105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the n‑th prime congruent to 2, 3 or 5 (mod 19) */

#include <stddef.h>          /* size_t */
#include <stdbool.h>         /* bool */
#include "../utils/is_prime.h"
#include "../utils/seq_realloc.h"

/* Cached primes satisfying the residue condition */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Next integer to test for primality */
static idx_t trial = 2;

/* Grow the cache until it contains at least n elements */
static void ensure_cache_len(idx_t n)
{
    while (cache_len < (size_t)n) {
        while (true) {
            if (trial < 2) trial = 2;
            idx_t r = trial % 19;
            if (r == 2 || r == 3 || r == 5) {
                if (is_prime(trial)) {
                    /* Ensure room in the array */
                    if (cache_len == cache_cap) {
                        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
                        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                        cache = tmp;
                        cache_cap = new_cap;
                    }
                    cache[cache_len++] = (seq_t)trial;
                    trial++;              /* advance to next candidate for future searches */
                    break;                /* prime stored – move to next required element */
                }
            }
            trial++;                      /* try next candidate */
        }
    }
}

/* Public interface requested by the harness */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* behaviour below offset is irrelevant */
    ensure_cache_len(n);
    return cache[(size_t)(n - 1)];
}
