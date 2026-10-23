/*
 * A044291  Numbers k such that the string 4,4 occurs in the base 9
 *          representation of k but not of k-1.
 * https://oeis.org/A044291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 801, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has44
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the n‑th integer k (k≥1) such that the base‑9
   representation of k contains the consecutive digits “44” while the
   base‑9 representation of k‑1 does not.  The sequence is A044291.  */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/has44.h"

/* Grow the cache array to at least 'need' elements.  */
static void grow_cache(size_t need, seq_t **cache, size_t *cap)
{
    size_t new_cap = (*cap > 0) ? (*cap << 1) : 128;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)
        return;  /* In the test harness memory will be available. */
    *cache = tmp;
    *cap   = new_cap;
}

seq_t a(idx_t n)
{
    /* Persistent state between calls. */
    static seq_t *cache      = NULL; /* Cached terms */
    static size_t cache_size = 0;    /* Number of terms cached */
    static size_t cache_cap  = 0;    /* Allocated array capacity */
    static seq_t next_k      = 1;    /* Next integer to test */

    /* Generate terms until we have at least n cached. */
    while (cache_size < (size_t)n) {
        seq_t k = next_k++;
        if (has44(k) && !has44(k - 1)) {
            if (cache_size == cache_cap)
                grow_cache(cache_size + 1, &cache, &cache_cap);
            cache[cache_size++] = k;
        }
    }
    return cache[n - 1];
}
