/*
 * A017890  Expansion of 1/(1-x^10-x^11-x^12-x^13-x^14).
 * https://oeis.org/A017890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-10) + a(n-11) + a(n-12) + a(n-13) + a(n-14) * for n >
 *            14, a(0)=1, a(1..9)=0, a(10..14)=1
 * Range:     u8 n <= 57, u16 n <= 100, u32 n <= 181, u64 n <= 345,
 *            u128 n <= 672, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* memoised recurrence */
seq_t a(idx_t n) {
    /* static cache for memoisation, grows as needed */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;     /* allocated capacity */
    static idx_t  last   = 0;      /* highest index computed so far */
    static int    done   = 0;      /* whether base values have been set once */

    const idx_t BASE_END = 14;     /* highest index with non‑recurrence value */

    /* ensure the cache can hold at least up to n and up to BASE_END */
    if (cap <= (size_t)n || cap <= (size_t)BASE_END) {
        size_t new_cap = cap ? cap * 2 : 1;
        /* grow until it can hold n and the base values */
        while (new_cap <= (size_t)n || new_cap <= (size_t)BASE_END)
            new_cap *= 2;

        /* reallocate and zero the newly allocated part */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failed */
        cache = tmp;
        size_t old = cap;
        for (size_t i = old; i < new_cap; ++i) cache[i] = 0;
        cap = new_cap;
    }

    /* initialise base values only once */
    if (!done) {
        cache[0] = 1;
        for (idx_t i = 1; i <= 9; ++i) cache[i] = 0;
        for (idx_t i = 10; i <= 14; ++i) cache[i] = 1;
        last = BASE_END;
        done = 1;
    }

    /* if the value is already computed, return it */
    if (n <= last) return cache[n];

    /* compute all new values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = cache[i-10] + cache[i-11] + cache[i-12] +
                   cache[i-13] + cache[i-14];
    }
    last = n;
    return cache[n];
}
