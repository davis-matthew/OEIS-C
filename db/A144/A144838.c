/*
 * A144838  a(n) = Lucas(6^n).
 * https://oeis.org/A144838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 18; a(n+1) = a(n)^6 - 6·a(n)^4 + 9·a(n)^2 - 2 (n ≥ 1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* 1‑based cache (index 0 unused) */
    static size_t cap   = 0;         /* current allocation size          */
    static size_t used  = 0;         /* last index computed (>=1 if set) */

    /* grow cache to accommodate index n */
    if (n > cap) {
        size_t new_cap = (cap < 2) ? 2 : cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) abort();      /* out of memory */
        if (used > 0) {
            for (size_t i = 0; i <= used; ++i)
                new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cap   = new_cap;
    }

    /* initialise the first term if not yet done */
    if (used == 0) {
        cache[1] = (seq_t)18;
        used = 1;
    }

    /* compute missing terms up to n (any order of calls works) */
    for (size_t i = used + 1; i <= (size_t)n; ++i) {
        seq_t x  = cache[i-1];
        seq_t x2 = x * x;          /* x^2 */
        seq_t x4 = x2 * x2;        /* x^4 */
        seq_t x6 = x4 * x2;        /* x^6 */
        seq_t val = x6 - (seq_t)6 * x4 + (seq_t)9 * x2 - (seq_t)2;
        cache[i] = val;
    }
    used = (size_t)n;

    return cache[n];
}
