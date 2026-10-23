/*
 * A129494  Composite numbers k such that 4^k mod k is a power of 4 greater
 *          than 1.
 * https://oeis.org/A129494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A129494: composite numbers k such that 4^k mod k is a power of 4 > 1. 
   The function returns the nth term (1‑based). */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/powmod64.h"

/* Cached values of the sequence: first (1‑based) term is a(1). */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t checked_upto = 0;

seq_t a(idx_t n) {
    /* Generate terms until we have at least n elements. */
    while (cache_len < n) {
        uint64_t k = ++checked_upto;
        /* Skip primes (and 1, though k starts at 4). */
        if (k < 4 || is_prime(k)) {
            continue;
        }
        /* Compute r = 4^k mod k using the provided 64‑bit routine. */
        uint64_t r = powmod64(4ULL, k, k);
        if (r <= 1) {
            continue; /* Not a power of 4 greater than 1. */
        }
        /* Test whether r is an exact power of 4. */
        uint64_t tmp = r;
        while (tmp % 4ULL == 0ULL) {
            tmp /= 4ULL;
        }
        if (tmp != 1ULL) {
            continue;
        }
        /* Store the qualifying composite number in the cache. */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) {
                /* Allocation failed – return something sensible. */
                return 0;
            }
            cache = new_cache;
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)k;
    }
    return cache[n - 1];
}
