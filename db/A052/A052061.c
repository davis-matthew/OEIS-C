/*
 * A052061  Numbers k such that decimal expansion of k^2 contains no
 *          palindromic substring except single digits.
 * https://oeis.org/A052061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1046, u32 n <= 9472, u64 n <= 9472,
 *            u128 n <= 9472, big any n
 * Helpers:   is_good_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A052061:
   a(n) = the n-th integer k whose square contains no palindromic
   decimal substring of length ≥ 2.  The sequence is indexed with
   offset 1, so a(1) is the first term (0).  The computation is
   performed by a brute‑force search, memoising found terms. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_good_candidate.h"

/* Function required by the build system.  Returns the n‑th term. */
seq_t a(idx_t n)
{
    /* Static cache of already found terms.  Size grows as needed. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Initialise cache on first call. */
    if (cache_len == 0) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failure */
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    /* Ensure cache can hold at least n terms. */
    if (n > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;             /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate new terms until the cache contains at least n entries. */
    while (cache_len < n) {
        seq_t cand = cache[cache_len - 1] + 1;
        while (!is_good_candidate(cand)) {
            /* Avoid infinite loop on overflow of fixed size types. */
            if (cand == 0) return 0;
            ++cand;
        }
        cache[cache_len] = cand;
        ++cache_len;
    }

    return cache[n - 1];
}
