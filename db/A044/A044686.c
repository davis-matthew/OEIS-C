/*
 * A044686  Numbers n such that string 6,0 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Procedure: Computes the nth term of OEIS A044686.
   A n such that the base‑9 representation contains the substring
   “60” but the base‑9 representation of n+1 does not. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Auxiliary: test whether a base‑9 number contains the substring 6,0. */
static bool has_60(seq_t x) {
    unsigned char digits[256];      /* enough for any seq_t (base‑9). */
    size_t len = 0;

    if (x == 0) return false;      /* “0” has no 60. */

    seq_t nine = (seq_t)9;
    while (x > 0) {
        digits[len++] = (unsigned char)(x % nine);
        x /= nine;
    }

    for (size_t i = 1; i < len; ++i)
        if (digits[i] == 6 && digits[i - 1] == 0)
            return true;
    return false;
}

/* Cache of previously computed terms to speed repeated calls. */
static seq_t *cache      = NULL;
static size_t  cache_cap = 0;
static size_t  cache_len = 0;

/* Grow the cache to at least 'need' elements. */
static void ensure_cache(size_t need) {
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    cache = mem_resize(seq_t, cache, new_cap);
    cache_cap = new_cap;
}

/* Main function required by the specification. */
seq_t a(idx_t n) {
    if (n == 0) return 0;        /* undefined for the OEIS offset, but keep safe */

    ensure_cache(n);

    /* Grow cache to contain at least 'n' terms. */
    seq_t candidate = 1;
    if (cache_len) candidate = cache[cache_len - 1] + 1;   /* start after last cached */

    while (cache_len < (size_t)n) {
        if (has_60(candidate) && !has_60(candidate + 1)) {
            /* store term */
            cache[cache_len++] = candidate;
        }
        ++candidate;
    }

    return cache[n - 1];
}
