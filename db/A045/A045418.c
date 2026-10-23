/*
 * A045418  Primes congruent to {0, 3, 4, 6} mod 7.
 * https://oeis.org/A045418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045418: primes p such that p mod 7 ∈ {0,3,4,6} */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t last_checked = 2;  /* start checking after 2 */

static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) return; /* allocation failed: keep old cache */
    if (cache) {
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n) return;
    /* This loop expands cache until it contains at least n elements */
    while (cache_len < (size_t)n) {
        if (!is_prime(last_checked)) { last_checked++; continue; }
        idx_t r = last_checked % 7;
        if (r == 0 || r == 3 || r == 4 || r == 6) {
            if (cache_len + 1 > cache_cap) grow_cache((cache_cap == 0) ? 128 : cache_cap * 2);
            cache[cache_len++] = (seq_t)last_checked;
        }
        last_checked++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset‑1, but return 0 safely */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
