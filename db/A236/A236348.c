/*
 * A236348  Expansion of (1 - x + 2*x^2 + x^3) / ((1 - x) * (1 - x^3)) in
 *          powers of x.
 * https://oeis.org/A236348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 254, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A236348:  Expansion of (1 - x + 2*x^2 + x^3) / ((1 - x) * (1 - x^3))
   Formula: a(n) = a(n-1) + a(n-3) - a(n-4)   (n ≥ 4)
   Initial values: a(0)=1, a(1)=0, a(2)=2, a(3)=4 */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoised values */
    static seq_t *cache = NULL;    /* array of computed terms */
    static size_t cap = 0;         /* current capacity of cache */
    static idx_t last = 3;         /* highest index that has been filled */

    /* initialise cache if first call */
    if (cache == NULL) {
        cap = 4; /* we need at least indices 0..3 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) { /* allocation failure – abort */
            return 0;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)4;
    }

    /* extend cache capacity if required */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap * 2;
        while ((size_t)n + 1 > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) { /* allocation failure – abort */
            return 0;
        }
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms up to index n (if any) */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            /* recurrence: a(i) = a(i-1) + a(i-3) - a(i-4) */
            cache[i] = cache[i - 1] + cache[i - 3] - cache[i - 4];
        }
        last = n;
    }

    return cache[n];
}
