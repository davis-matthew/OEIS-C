/*
 * A075913  Fifth column of triangle A075500.
 * https://oeis.org/A075913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 75*a(n-1) - 2125*a(n-2) + 28125*a(n-3) - 171250*a(n-4) +
 *            375000*a(n-5) with initial terms a(0)=1, a(1)=75, a(2)=3500,
 *            a(3)=131250, a(4)=4344375
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A075913 – fifth column of triangle A075500 */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Initial five terms */
    static const seq_t init[5] = {
        (seq_t)1,
        (seq_t)75,
        (seq_t)3500,
        (seq_t)131250,
        (seq_t)4344375
    };

    /* Cache to allow random‑order calls and avoid recomputation */
    static seq_t *cache = NULL;
    static idx_t cached_last = 0;   /* highest index already computed (>=0) */
    static size_t cache_cap = 0;    /* allocated size */

    /* Allocate initial cache if not yet done */
    if (cache == NULL) {
        cache_cap = 5;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 5; ++i) cache[i] = init[i];
        cached_last = 4;
    }

    /* Extend cache if needed for requested n */
    if (n > cached_last) {
        /* Grow cache to at least n+1 */
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap = new_cap * 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;

        /* Compute new terms up to n */
        for (idx_t k = cached_last + 1; k <= n; ++k) {
            /* a(k) = 75*a(k-1) - 2125*a(k-2) + 28125*a(k-3)
                      - 171250*a(k-4) + 375000*a(k-5) */
            seq_t t1 = (seq_t)75 * cache[k-1];
            seq_t t2 = (seq_t)2125 * cache[k-2];
            seq_t t3 = (seq_t)28125 * cache[k-3];
            seq_t t4 = (seq_t)171250 * cache[k-4];
            seq_t t5 = (seq_t)375000 * cache[k-5];
            cache[k] = t1 - t2 + t3 - t4 + t5;
        }
        cached_last = n;
    }

    return cache[n];
}
