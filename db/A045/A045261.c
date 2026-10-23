/*
 * A045261  Numbers whose base-5 representation contains exactly three 1's and
 *          two 4's.
 * https://oeis.org/A045261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer whose base‑5 expansion contains exactly
 *            three 1’s and two 4’s (OEIS A045261)
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Cache for previously found values.  The cache grows only as needed
   and is shared among all calls to a(). */
static seq_t *cache          = NULL;
static size_t cache_cap      = 0;
static size_t cache_len      = 0;
static seq_t cursor          = 1;   /* next integer to test */

/* Grow the cache when it is full. */
static void grow_cache(void) {
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* In a proper build environment we could abort or
           handle the allocation failure more gracefully. */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Append one value to the cache. */
static void push_cache(seq_t val) {
    if (cache_len == cache_cap) grow_cache();
    cache[cache_len++] = val;
}

/* Return true iff val has exactly three 1‑digits and two 4‑digits
   in its base‑5 representation.  The test stops early if the
   required counts are exceeded. */
static bool has_property(seq_t val) {
    idx_t tmp = (idx_t)val;      /* val will never exceed idx_t range */
    int c1 = 0, c4 = 0;
    while (tmp > 0) {
        int d = tmp % 5;
        if (d == 1) {
            ++c1;
            if (c1 > 3) return false;
        } else if (d == 4) {
            ++c4;
            if (c4 > 2) return false;
        }
        tmp /= 5;
    }
    return c1 == 3 && c4 == 2;
}

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* out of bounds – undefined, but safe */

    /* Extend the cache until it contains at least n entries. */
    while (cache_len < n) {
        /* Find the next integer that satisfies the property. */
        while (true) {
            if (has_property(cursor)) {
                push_cache(cursor);
                ++cursor;            /* next candidate after the one we just used */
                break;
            }
            ++cursor;
        }
    }
    return cache[n - 1];
}
