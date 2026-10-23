/*
 * A044765  Numbers n such that string 5,2 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains_52
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044765: numbers n that contain the decimal substring "52" but n+1 does not.
   The routine searches sequentially, caching the results for later calls. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/contains_52.h"

/* Return the n-th term (1‑based). */
seq_t a(idx_t n)
{
    /* Static cache for already computed terms */
    static seq_t *cache        = NULL;
    static size_t cache_size   = 0;   /* allocated elements */
    static size_t cache_used   = 0;   /* number of terms stored */
    static seq_t candidate     = 52;  /* next number to check (initialized once) */

    /* Allocate the cache on the first call. */
    if (cache == NULL) {
        cache_size = 16;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) abort();  /* unrecoverable */
    }

    /* Extend the cache until it contains at least n terms. */
    while (cache_used < n) {
        /* Find the next integer that satisfies the condition. */
        while (!(contains_52(candidate) && !contains_52(candidate + (seq_t)1))) {
            candidate++;
        }

        /* Store the found term. */
        if (cache_used == cache_size) {
            cache_size *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
            if (tmp == NULL) abort();
            cache = tmp;
        }
        cache[cache_used++] = candidate++;
    }

    return cache[n - 1];
}
