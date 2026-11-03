/*
 * A084697  a(1) = 2; for n >= 1, k>=1, a(n+1) = a(n) + k*n is the smallest
 *          such prime.
 * https://oeis.org/A084697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; for n >= 1, a(n+1) is the smallest prime > a(n) such
 *            that a(n+1) ≡ a(n) (mod n)
 * Range:     u8 n <= 11, u16 n <= 151, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 974 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

/* Static cache to store the sequence terms. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;          /* current capacity of cache */
static idx_t  cache_last = 0;         /* index of last computed term */

/* Computes the n-th term of A084697 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n == 0, but safe */

    /* Ensure the cache can hold indices up to n. */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialize the first term once. */
    if (cache_last == 0) {
        cache[1] = 2;
        cache_last = 1;
    }

    /* Compute missing terms up to n. */
    while (cache_last < n) {
        idx_t   mod   = cache_last;            /* m = current n */
        seq_t   prev  = cache[cache_last];
        seq_t   cand  = prev + mod;            /* k = 1 */

        /* Search for the next prime congruent to prev modulo mod. */
        while (!is_prime((idx_t)cand)) {
            cand += mod;
        }
        cache[++cache_last] = cand;           /* cache[n] */
    }

    return cache[n];
}
