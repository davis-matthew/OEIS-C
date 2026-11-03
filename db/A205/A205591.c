/*
 * A205591  a(1) = 1, a(n) = a(floor((2n-1)/3)) + a(floor(2n/3)) for n > 1.
 * https://oeis.org/A205591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 77 terms through n = 981 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(1) = 1; a(n) = a(floor((2n-1)/3)) + a(floor(2n/3)) for n > 1 */
#include <stddef.h>
#include <stdlib.h>
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

/* static cache for memoized values; 0 means uncomputed (all terms are positive) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Recursive computation with memoisation */
seq_t a(idx_t n)
{
    if (n == 1)          /* base case */
        return 1;

    /* make sure the cache can hold index n */
    ensure_cache(n, &cache, &cache_cap);

    /* return already computed value if present */
    if (cache[n] != 0)
        return cache[n];

    /* compute recursively */
    idx_t k1 = (2*n - 1) / 3;
    idx_t k2 = (2*n)     / 3;
    seq_t v1 = a(k1);
    seq_t v2 = a(k2);
    seq_t result = v1 + v2;

    /* store for future calls */
    cache[n] = result;
    return result;
}
