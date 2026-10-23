/*
 * A137012  Numbers k such that k and k^2 use only the digits 1, 2, 5 and 9.
 * https://oeis.org/A137012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   next_allowed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers k such that k and k^2 use only digits 1, 2, 5 and 9. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_allowed.h"

/* Return true iff all decimal digits of x are 1, 2, 5, or 9. */
static bool allowed_digits(seq_t x)
{
    /* For zero, treat as allowed (not used in this sequence). */
    if (x == (seq_t)0)
        return true;
    while (x > (seq_t)0) {
        seq_t d = x % (seq_t)10;
        if (d != (seq_t)1 && d != (seq_t)2 && d != (seq_t)5 && d != (seq_t)9)
            return false;
        x /= (seq_t)10;
    }
    return true;
}

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for offset 0 */

    /* Static cache of already found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of stored terms */
    static size_t cache_cap = 0;   /* capacity of cache array */
    static seq_t next_candidate = 0;   /* last processed allowed‑digit number */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        seq_t cand = next_allowed(next_candidate);
        seq_t sq = cand * cand;
        if (allowed_digits(sq)) {
            /* Append cand to cache. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
        /* Prepare for next iteration. */
        next_candidate = cand;
    }

    return cache[n - 1];
}
