/*
 * A044581  Numbers k such that string 1,5 occurs in the base 8 representation
 *          of k but not of k+1.
 * https://oeis.org/A044581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 977 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A044581: a(n) = k such that the base‑8 representation of k contains the
   substring "15" but the base‑8 representation of (k+1) does not. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Return true if v in base 8 contains the substring 15. */
static bool has15(seq_t v)
{
    if (v == (seq_t)0)
        return false;

    /* Store the octal digits (least significant first). */
    uint8_t digits[64];          /* 64 digits suffice for 128‑bit values. */
    int pos = 0;
    seq_t t = v;
    while (t != (seq_t)0) {
        digits[pos++] = (uint8_t)(t % (seq_t)8);
        t = t / (seq_t)8;
    }

    /* Scan from most significant digit to least. */
    for (int i = pos - 1; i > 0; --i) {
        if (digits[i] == 1 && digits[i - 1] == 5)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Static cache to allow O(n) time with arbitrary order of calls. */
    static seq_t *cache     = NULL;
    static size_t  len      = 0;   /* number of terms currently stored */
    static size_t  cap      = 0;   /* allocated capacity of cache */
    static seq_t  next      = (seq_t)0;   /* last number examined */

    /* Initialise the cache on first call. */
    if (cache == NULL) {
        cap   = 32;
        cache = mem_new(seq_t, cap);
        if (cache == NULL)
            exit(EXIT_FAILURE);
    }

    /* Generate terms until we have at least n entries. */
    while (len < n) {
        /* Grow capacity if necessary. */
        if (len + 1 > cap) {
            size_t new_cap = cap * 2;
            seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
            if (new_cache == NULL)
                exit(EXIT_FAILURE);
            cache = new_cache;
            cap   = new_cap;
        }

        /* Check the next integer. */
        seq_t k = next + (seq_t)1;
        next = k;

        if (has15(k) && !has15(k + (seq_t)1))
            cache[len++] = k;
    }

    return cache[n - 1];
}
