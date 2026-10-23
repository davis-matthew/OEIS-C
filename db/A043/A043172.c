/*
 * A043172  Numbers k such that 3 and 6 occur juxtaposed in the base-8
 *          representation of k but not of k-1.
 * https://oeis.org/A043172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer k such that the base‑8
 *            representation of k contains adjacent 3 and 6 (either 36 or 63)
 *            but the representation of k−1 does not. Computed by a simple
 *            brute‑force search with memoisation
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_adjacent_36
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/has_adjacent_36.h"

/* Global cache of found terms.  Shared between a() and
   cache growth helper. */
static seq_t *seq_cache = NULL;
static size_t seq_cache_cap = 0;

/* Ensure that seq_cache can store at least 'need' elements. */
static void grow_cache(size_t need)
{
    if (need <= seq_cache_cap)
        return;

    size_t new_cap = (seq_cache_cap == 0u) ? 16u : seq_cache_cap * 2u;
    while (new_cap < need)
        new_cap <<= 1u;

    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = tmp;
    seq_cache_cap = new_cap;
}

/* External linkage: compute the nth term of the sequence. */
seq_t a(idx_t n)
{
    static size_t len = 0u;          /* number of cached terms */
    static idx_t next_candidate = 1; /* next integer to test */

    if (n == 0u)  /* offset is 1, but guard against invalid input */
        return (seq_t)0u;

    /* Generate terms until we have at least n of them. */
    while (len < n) {
        seq_t cur = (seq_t)next_candidate;
        seq_t prev = cur - (seq_t)1u;    /* k-1 in seq_t */

        if (has_adjacent_36(cur) && !has_adjacent_36(prev)) {
            grow_cache(len + 1u);
            seq_cache[len++] = cur;
        }
        ++next_candidate;
    }

    return seq_cache[n - 1u];
}
