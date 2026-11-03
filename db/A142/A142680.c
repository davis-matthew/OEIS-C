/*
 * A142680  Primes congruent to 25 mod 57.
 * https://oeis.org/A142680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 177, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 935 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence : prim<sup>ed</sup> numbers congruent to 25 mod 57 (OEIS A142680) */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
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

/* a(n) = the n-th prime p with p ≡ 25 (mod 57), indexed from n = 1 */
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;          /* cached values */
    static size_t cache_size = 0;        /* number of cached elements */
    static size_t cache_cap  = 0;        /* capacity of cache array */
    static idx_t next_candidate = 139;   /* first candidate to test */

    /* if value already computed */
    if (n <= cache_size)
        return cache[n - 1];

    /* ensure cache capacity for at least n elements */
    if (n > cache_cap)
        ensure_cache(n, &cache, &cache_cap);

    /* generate further primes congruent to 25 (mod 57) */
    idx_t cand = next_candidate;
    while (cache_size < n) {
        if (is_prime(cand))
            cache[cache_size++] = (seq_t)cand;
        cand += 57;
    }
    /* remember last tested candidate plus 57 for next round */
    next_candidate = cand;

    return cache[n - 1];
}
