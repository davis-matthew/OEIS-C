/*
 * A072543  Numbers whose largest decimal digit is also the initial digit.
 * https://oeis.org/A072543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 7733, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   check_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A072543: numbers whose largest decimal digit is also the initial digit.
   The function enumerates integers in increasing order and collects those where the most
   significant digit equals the maximum digit in the number. */
#include <stdlib.h>
#include "../utils/check_candidate.h"

seq_t a(idx_t n)
{
    /* static cache of computed terms: zero‑based index */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 0;      /* next integer to test */

    /* expand cache until we have at least n terms */
    while (cache_len < n) {
        /* grow the cache if necessary */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) { /* allocation failure – abort */
                abort();
            }
            cache = new_cache;
            cache_cap = new_cap;
        }
        /* test the next candidate number */
        if (check_candidate(next_candidate)) {
            cache[cache_len] = (seq_t)next_candidate;
            cache_len++;
        }
        next_candidate++;
    }

    /* return the requested term (n is 1‑based) */
    return cache[n - 1];
}
