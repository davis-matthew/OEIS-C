/*
 * A039154  Numbers whose base-10 representation has the same number of 7's
 *          and 9's.
 * https://oeis.org/A039154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n-th non‑negative integer whose base‑10
 *            representation contains an equal number of digit 7 and digit 9
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether a number has the same count of 7’s and 9’s. */
static inline bool has_equal_sevens_nines(seq_t x)
{
    int c7 = 0, c9 = 0;
    seq_t t = x;
    while (t > 0) {
        int d = (int)(t % 10);
        if (d == 7) ++c7;
        else if (d == 9) ++c9;
        t /= 10;
    }
    return c7 == c9;
}

/* Return the n‑th term of A039154 (1‑based index). */
seq_t a(idx_t n)
{
    /* Static cache of already computed terms. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t cached = 0;   /* number of terms stored in the cache */
    static seq_t next   = 0;   /* candidate value to test next */

    /* Lazy allocation of the cache. */
    if (!cache) {
        cap   = 16;
        cache = mem_new(seq_t, cap);
        if (!cache) return 0;  /* allocation failure → fallback */
    }

    /* Grow cache until it contains at least n terms. */
    while (cached < n) {
        /* Find next valid number. */
        while (!has_equal_sevens_nines(next))
            ++next;

        /* Grow array if necessary. */
        if ((size_t)cached == cap) {
            cap *= 2;
            seq_t *tmp = mem_resize(seq_t, cache, cap);
            if (!tmp) return 0; /* allocation failure → fallback */
            cache = tmp;
        }
        cache[cached++] = next;
        ++next;
    }
    return cache[n - 1];
}
