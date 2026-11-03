/*
 * A033561  Primes p such that 6!+p is also prime.
 * https://oeis.org/A033561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) returns the nth prime p such that p and p+720 are prime
 * Range:     u8 n <= 24, u16 n <= 2293, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9422 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static idx_t last_checked = 2;   /* start at the smallest prime candidate */

    /* initialise cache lazily */
    if (cache == NULL) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        cache_len = 0;
    }

    const idx_t shift = (idx_t)720;   /* 6! */

    /* grow cache until we have at least n terms */
    while (cache_len < n) {
        idx_t p = last_checked;

        /* find next prime >= last_checked */
        while (!is_prime(p))
            ++p;

        /* test if p + 720 is also prime */
        if (is_prime(p + shift)) {
            /* store p in cache, resizing if necessary */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = (seq_t)p;
        }

        /* next search starts after the prime we just examined */
        last_checked = p + 1;
    }

    /* return the requested term (1‑based index) */
    return cache[(size_t)(n - 1)];
}
