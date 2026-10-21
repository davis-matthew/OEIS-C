/*
 * A213780  Antidiagonal sums of the convolution array A213778.
 * https://oeis.org/A213780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 5*a(n-3) + 5*a(n-4) + a(n-5) -
 *            3*a(n-6) + a(n-7) Initial terms: a(1)=1, a(2)=6, a(3)=17,
 *            a(4)=40, a(5)=78, a(6)=140, a(7)=230
 * Range:     u8 n <= 7, u16 n <= 33, u32 n <= 564, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>


seq_t a(idx_t n)
{
    /* static cache of computed terms, 1‑indexed */
    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* initialise cache with first 7 terms */
    if (cache == NULL) {
        cap = 7;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* memory failure, result undefined */
        const seq_t init[7] = {(seq_t)1, (seq_t)6, (seq_t)17,
                               (seq_t)40, (seq_t)78, (seq_t)140,
                               (seq_t)230};
        for (size_t i = 0; i < 7; ++i) cache[i] = init[i];
    }

    /* if requested index is within cache, return it */
    if ((size_t)n <= cap)
        return cache[n - 1];

    /* grow cache to hold up to n */
    size_t old_cap = cap;
    size_t new_cap = (size_t)n;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (new_cache == NULL) return (seq_t)0; /* memory failure */
    for (size_t i = 0; i < old_cap; ++i)
        new_cache[i] = cache[i];
    free(cache);
    cache = new_cache;
    cap = new_cap;

/* compute missing terms using the 7‑term linear recurrence */
    for (idx_t i = old_cap + 1; i <= n; ++i) {
        seq_t val = 3 * cache[i - 2]          /* a(i-1) */
                    - cache[i - 3]           /* a(i-2) */
                    - 5 * cache[i - 4]       /* a(i-3) */
                    + 5 * cache[i - 5]       /* a(i-4) */
                    + cache[i - 6]           /* a(i-5) */
                    - 3 * cache[i - 7]       /* a(i-6) */
                    + cache[i - 8];          /* a(i-7) */
        cache[i - 1] = val;
    }

    return cache[n - 1];
}
