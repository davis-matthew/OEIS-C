/*
 * A044143  Numbers k such that string 0,5 occurs in the base 7 representation
 *          of k but not of k-1.
 * https://oeis.org/A044143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1506, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   contains05_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the n‑th integer k such that the base‑7 representation
   of k contains the substring "05" while the representation of k-1 does not. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/contains05_idx.h"

seq_t a(idx_t n)
{
    /* Offset is 1 – the function is never called with n == 0. */
    if (n == 0) return 0;

    static seq_t *cache   = NULL;   /* cache of previously found terms */
    static idx_t cache_len = 0;     /* number of terms stored */
    static idx_t cache_cap = 0;     /* allocated capacity of cache */
    static idx_t next_k = 1;        /* next candidate to test */

    while (cache_len < n) {
        /* Find the next k that satisfies the condition. */
        while (!(contains05_idx(next_k) && !contains05_idx(next_k - 1)))
            ++next_k;

        /* Ensure sufficient capacity in the cache. */
        if (cache_len == cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) abort();   /* out of memory */
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* Store the found term (casting to seq_t). */
        cache[cache_len++] = (seq_t)next_k;
        ++next_k;                /* start next search from the following number */
    }

    return cache[n - 1];
}
