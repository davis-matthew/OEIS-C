/*
 * A043885  Numbers k such that 2 and 3 occur juxtaposed in the base-4
 *          representation of k but not of k+1.
 * https://oeis.org/A043885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 4214, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_23_or_32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iteratively find numbers k such that k's base‑4
   representation contains an adjacent 2 and 3 (in either order)
   while (k+1) does not.  The sequence is indexed from 1. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/has_23_or_32.h"

/* Cache for already found sequence terms. */
static seq_t *cache = NULL;   /* pointer to array of seq_t values */
static size_t cache_cap = 0;   /* allocated capacity of the cache   */
static size_t cache_len = 0;   /* number of terms currently stored  */
static seq_t last_tested = 0;  /* last integer examined for pattern */

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for non‑positive indices */

    /* Grow the cache until it holds at least n terms. */
    while (cache_len < n) {
        ++last_tested;  /* next candidate k */

        if (has_23_or_32(last_tested) &&
            !has_23_or_32(last_tested + 1)) {

            /* Ensure enough space in the cache. */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 8 : cache_cap << 1;
                seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
                if (!tmp) abort();
                if (cache) {
                    memcpy(tmp, cache, cache_len * sizeof(seq_t));
                    free(cache);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = last_tested;
        }
    }

    return cache[n - 1];
}
