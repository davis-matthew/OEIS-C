/*
 * A043141  Numbers k such that 1 and 6 occur juxtaposed in the base-7
 *          representation of k but not of k-1.
 * https://oeis.org/A043141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th positive integer k such that the base‑7
 *            representation of k contains the substring 16 or 61, while the
 *            representation of k‑1 contains neither
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   has_adjacent_16_61_base7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine relies only on basic integer arithmetic and comparison. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/has_adjacent_16_61_base7.h"

/* Compute the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)                     /* offset 1: undefined for n==0 */
        return 0;

    static seq_t *cache = NULL;      /* cached sequence values */
    static idx_t cache_size = 0;     /* number of cached terms */
    static idx_t cache_cap = 0;      /* allocated capacity */
    static seq_t next_k = 1;         /* next integer to test after the last cache hit */

    /* Grow the cache until it contains at least n terms. */
    while (cache_size < n) {
        seq_t k = next_k;
        for (;;) {
            if (has_adjacent_16_61_base7(k) &&
                !has_adjacent_16_61_base7(k - 1)) {
                /* Ensure enough room in the cache. */
                if (cache_size == cache_cap) {
                    size_t new_cap = cache_cap == 0 ? 16 : cache_cap << 1;
                    seq_t *newp = (seq_t *)realloc(cache,
                                                    new_cap * sizeof(seq_t));
                    /* In production code we should check newp==NULL. */
                    cache = newp;
                    cache_cap = (idx_t)new_cap;
                }
                cache[cache_size++] = k;
                next_k = k + 1;      /* start next search after this k */
                break;
            }
            ++k;                   /* try the next candidate */
        }
    }

    return cache[n - 1];
}
