/*
 * A039461  Numbers whose base-11 representation has the same nonzero number
 *          of 2's and 7's.
 * https://oeis.org/A039461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Compute the nth term of OEIS sequence A039461:
 * Numbers whose base-11 representation has the same non‑zero number
 * of 2's and 7's.
 *
 * The function performs a brute‑force search, caching results
 * so that successive calls are efficient.  It works for any
 * integer type specified as seq_t and idx_t by the build.
 */

#include <stdlib.h>   /* malloc, free, size_t */

static int is_candidate(seq_t v) {
    int cnt2 = 0;
    int cnt7 = 0;
    seq_t tmp = v;
    while (tmp > 0) {
        unsigned d = (unsigned)(tmp % 11);
        if (d == 2) ++cnt2;
        else if (d == 7) ++cnt7;
        tmp = tmp / 11;
    }
    return (cnt2 > 0 && cnt2 == cnt7);
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;   /* offset 1 – behaviour below is irrelevant */

    /* Cached results */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of cached terms */
    static size_t cache_cap  = 0;   /* allocated capacity */
    static seq_t next_candidate = 1; /* next integer to test */

    if (!cache) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) {
            /* Allocation failed – return 0 as a fallback */
            return (seq_t)0;
        }
        cache_size = 0;
    }

    /* Extend cache until we have at least n terms */
    while (cache_size < n) {
        /* Find next valid candidate */
        while (!is_candidate(next_candidate)) ++next_candidate;
        /* Store it */
        if (cache_size == cache_cap) {
            cache_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* realloc failed */
            cache = tmp;
        }
        cache[cache_size++] = next_candidate;
        ++next_candidate;
    }

    return cache[n - 1];
}
