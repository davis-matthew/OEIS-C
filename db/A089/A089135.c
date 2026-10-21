/*
 * A089135  a(n) = -a(n-2) + 2*a(n-4) - a(n-10).
 * https://oeis.org/A089135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = -a(n-2) + 2*a(n-4) - a(n-10) with initial values: a(1)=1,
 *            a(2)=0, a(3)=0, a(4)=2, a(5)=3, a(6)=-2, a(7)=-3, a(8)=6,
 *            a(9)=9, a(10)=-11 The recurrence holds for all n ≥ 11
 * Range:     i8 n <= 16, i16 n <= 32, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cap = 0;       /* capacity of cache array */
    static idx_t computed = 0;  /* highest index computed so far */

    /* first 10 terms, index 0 unused */
    static const seq_t init[11] = { 0,
        1,   /* a(1) */
        0,   /* a(2) */
        0,   /* a(3) */
        2,   /* a(4) */
        3,   /* a(5) */
        -2,  /* a(6) */
        -3,  /* a(7) */
        6,   /* a(8) */
        9,   /* a(9) */
        -11  /* a(10) */
    };

    /* special case: first 10 terms are known */
    if (n <= 10 && n >= 1)
        return init[(idx_t)n];

    /* enlarge cache as needed */
    if (cache == NULL) {
        cap = 16;  /* initial capacity, arbitrary */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    } else if (n + 1 > cap) {
        /* double until we have enough room */
        while (n + 1 > cap)
            cap <<= 1;  /* multiply by 2 */
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }

    /* compute missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i <= 10) {
            /* use pre‑computed values for the first 10 indices */
            cache[i] = init[(idx_t)i];
        } else {
            /* recurrence: a(i) = -a(i-2) + 2*a(i-4) - a(i-10) */
            cache[i] = -cache[i - 2] + 2 * cache[i - 4] - cache[i - 10];
        }
    }
    computed = n;  /* update largest computed index */
    return cache[n];
}
