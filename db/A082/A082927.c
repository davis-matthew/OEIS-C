/*
 * A082927  Numbers in which at least one pair of adjacent digits differ by 1.
 * https://oeis.org/A082927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: enumerate integers in increasing order that contain at least
   one pair of adjacent decimal digits with absolute difference 1.
   The subsequence starts with 10, 12, 21, 23, 32, …  (offset 1). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Convert a 64‑bit integer to seq_t and test if the number has an adjacent
   digit pair differing by 1.  The function operates purely on idx_t values. */
static bool has_adjacent_diff_one(idx_t x)
{
    /* Numbers with fewer than two digits cannot satisfy the property. */
    if (x < 10) return false;

    int prev = x % 10;
    x /= 10;
    while (x > 0) {
        int curr = x % 10;
        int diff = curr > prev ? curr - prev : prev - curr;
        if (diff == 1) return true;
        prev = curr;
        x /= 10;
    }
    return false;
}

/* Dynamic cache of already found sequence terms. */
static seq_t *cache = NULL;
static size_t cache_len   = 0;   /* number of terms stored          */
static size_t cache_cap   = 0;   /* allocated capacity in items      */
static idx_t  next_candidate = 10;  /* next integer to check          */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_cache_cap(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();          /* insufficient memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return the nth term of A082927 (offset 1).  The function lazily computes
   terms on demand and stores them in a static cache, so repeated calls
   (in any order) become efficient. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1, but guard against zero */

    ensure_cache_cap(n);

    while (cache_len < n) {
        if (has_adjacent_diff_one(next_candidate)) {
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
