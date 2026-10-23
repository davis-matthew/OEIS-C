/*
 * A044166  Numbers n such that string 4,0 occurs in the base 7 representation
 *          of n but not of n-1.
 * https://oeis.org/A044166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   contains_40
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044166: numbers n whose base-7 representation contains "40" but n-1 does not.
   Computed by sequential search with memoization. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/contains_40.h"

/* Cache for the sequence values. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Ensure the cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        /* Grow capacity if necessary. */
        if (cache_len >= cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Start searching from the next integer after the last stored term. */
        seq_t candidate = (cache_len == 0) ? (seq_t)1
                                          : cache[cache_len - 1] + (seq_t)1;

        for (;;) {
            if (contains_40(candidate) && !contains_40(candidate - (seq_t)1)) {
                cache[cache_len++] = candidate;
                break;
            }
            candidate++;
        }
    }
}

/* Return the n‑th term of A044166 (a(1) = 28). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (cache_len < n)
        ensure_cache(n);
    return cache[n - 1];
}
