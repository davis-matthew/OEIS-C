/*
 * A200764  Number of 0..n arrays x(0..3) of 4 elements with nondecreasing
 *          average value.
 * https://oeis.org/A200764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 3*a(n-3) - a(n-4) + a(n-5) + 3*a(n-6)
 *            - a(n-7) - 2*a(n-8) + a(n-9) Initial values: a(1)=5, a(2)=19,
 *            a(3)=51, a(4)=113, a(5)=219, a(6)=388, a(7)=638, a(8)=995,
 *            a(9)=1483
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    static seq_t *cache = NULL;
    static idx_t max_computed = 0;

    static const seq_t init[10] = {
        0,          /* a(0) unused */
        5,          /* a(1) */
        19,         /* a(2) */
        51,         /* a(3) */
        113,        /* a(4) */
        219,        /* a(5) */
        388,        /* a(6) */
        638,        /* a(7) */
        995,        /* a(8) */
        1483        /* a(9) */
    };

    /* Allocate cache and load initial values if not yet done */
    if (cache == NULL) {
        size_t cap = (size_t)((n > 9) ? n : 9) + 1;   /* indices 0..n */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;       /* out‑of‑memory fallback */
        for (idx_t i = 0; i <= 9; ++i) cache[i] = init[i];
        max_computed = 9;
    }

    /* Extend and compute terms up to n if needed */
    if (n > max_computed) {
        size_t needed = (size_t)(n + 1);
        if (needed > max_computed + 1) {
            cache = (seq_t*)realloc(cache, needed * sizeof(seq_t));
            if (!cache) return 0;   /* out‑of‑memory fallback */
        }
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* Positive grouping to avoid unsigned underflow */
            seq_t term = cache[i - 1] * (seq_t)2
                        + cache[i - 2]            + cache[i - 5]
                        + cache[i - 6] * (seq_t)3 + cache[i - 9]
                        - (cache[i - 3] * (seq_t)3
                           + cache[i - 4]
                           + cache[i - 7]
                           + cache[i - 8] * (seq_t)2);
            cache[i] = term;
        }
        max_computed = n;
    }

    return cache[n];
}
