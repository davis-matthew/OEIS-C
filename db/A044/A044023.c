/*
 * A044023  Numbers k such that 3 and 7 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   contains_adjacent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044023: Numbers k such that 3 and 7 occur juxtaposed in the base‑10 representation of k
 * but not of k+1.  The implementation enumerates integers, keeping a cache so that
 * a(n) can be returned in O(1) after the first few calls. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/contains_adjacent.h"

seq_t a(idx_t n)
{
    /* Static cache to support arbitrary order of calls. */
    static seq_t *cache = NULL;      /* array of found terms */
    static size_t cache_len = 0;     /* number of terms stored */
    static size_t cache_cap = 0;     /* allocated capacity */
    static bool initialized = false; /* ensure we start from 37 only once */
    static seq_t next_candidate = 37;/* next integer to examine */

    if (!initialized) {
        initialized = true;
        next_candidate = 37;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        seq_t k = next_candidate;
        if (contains_adjacent(k)) {
            seq_t kp1 = k + 1;
            if (!contains_adjacent(kp1)) {
                /* Ensure capacity. */
                if (cache_len >= cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort(); /* allocation failed */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = k;
            }
        }
        next_candidate = k + 1;
    }

    return cache[n - 1];
}
