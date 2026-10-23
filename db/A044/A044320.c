/*
 * A044320  Numbers k such that the string 7,6 occurs in the base 9
 *          representation of k but not of k-1.
 * https://oeis.org/A044320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 890, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_76, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044320:
   a(n) is the nth integer k such that the base‑9 representation of k
   contains the substring 7,6 while the base‑9 representation of k‑1
   does not.  The sequence is indexed with offset 1. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"
#include "../utils/has_76.h"

seq_t a(idx_t n)
{
    if (n < (idx_t)1)          /* offset is 1; undefined for n < 1 */
        return (seq_t)0;

    static seq_t *cache = NULL;      /* memoised list of terms   */
    static size_t cache_len = 0;     /* how many terms stored    */
    static size_t cache_cap = 0;     /* allocated capacity       */
    static seq_t next_candidate = (seq_t)1; /* next integer to test */

    /* Grow the cache until it holds at least n terms. */
    while (cache_len < (size_t)n) {
        seq_t cand = next_candidate;

        if (has_76(cand) && !has_76(cand - (seq_t)1)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (tmp == NULL)
                    abort();        /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }

        /* advance to the next candidate; safe for 64‑bit seq_t,
           long numbers will wrap only after all terms are found. */
        next_candidate = cand + (seq_t)1;
    }

    return cache[(size_t)n - 1];
}
