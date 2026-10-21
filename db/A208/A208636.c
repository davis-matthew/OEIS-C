/*
 * A208636  Number of n X 7 0..1 arrays with new values 0..1 introduced in row
 *          major order and no element equal to more than one of its immediate
 *          leftward or upward or right-upward antidiagonal neighbors.
 * https://oeis.org/A208636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 21*a(n-2) + 20*a(n-3) - 5*a(n-4) with a(1)=64,
 *            a(2)=191, a(3)=628, a(4)=2149 (OEIS A208636)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 32, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence satisfies a linear homogeneous recurrence of order 4.
   We memoize the terms so that any call order runs in amortised O(1). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of computed terms, 1‑based */
    static seq_t *cache = NULL;
    static idx_t   max_n  = 0;
    static size_t  cap    = 0;

    /* initialise cache with first four terms */
    if (cache == NULL) {
        cap = 8;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) abort();        /* fatal in unlikely event of OOM */

        cache[1] = 64;
        cache[2] = 191;
        cache[3] = 628;
        cache[4] = 2149;
        max_n = 4;
    }

    /* expand cache if necessary */
    if (n > max_n) {
        /* compute missing terms up to n */
        for (idx_t i = max_n + 1; i <= n; ++i) {

            /* indices are 1‑based; use previous values */
            seq_t a1 = cache[i-1];
            seq_t a2 = cache[i-2];
            seq_t a3 = cache[i-3];
            seq_t a4 = cache[i-4];

            /* use a numerically stable recombination: */
            /* a(i) = (8*a(i-1) + 20*a(i-3)) - (21*a(i-2) + 5*a(i-4)) */
            seq_t left  = a1 * (seq_t)8 + a3 * (seq_t)20;
            seq_t right = a2 * (seq_t)21 + a4 * (seq_t)5;
            seq_t next  = left - right;

            /* grow array if needed */
            if ((size_t)i >= cap) {
                size_t new_cap = cap * 2;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* OOM */
                cache = tmp;
                cap = new_cap;
            }

            cache[i] = next;
        }
        max_n = n;
    }

    return cache[n];
}
