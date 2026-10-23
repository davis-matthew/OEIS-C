/*
 * A032980  Numbers with the property that all pairs of consecutive base-9
 *          digits differ by 0 or 1.
 * https://oeis.org/A032980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 870, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   valid_base9_adj
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A032980: numbers whose base‑9 digits differ by 0 or 1 between
   consecutive digits. Uses brute‑force enumeration with memoization. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/valid_base9_adj.h"

static seq_t *cache = NULL;      /* Array of previously found terms. */
static size_t  cache_len = 0;    /* Number of stored terms. */
static size_t  cache_cap = 0;    /* Allocated capacity. */
static seq_t   next_candidate = 1;  /* Next number to test. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* Undefined, but harmless. */

    /* Expand cached sequence until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        /* Find the next valid number. */
        while (true) {
            if (valid_base9_adj(next_candidate)) {
                /* Grow the cache if necessary. */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();          /* Allocation failed. */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = next_candidate;
                break;
            }
            ++next_candidate;
        }
        ++next_candidate;
    }

    return cache[(size_t)n - 1];
}
