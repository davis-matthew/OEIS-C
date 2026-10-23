/*
 * A183246  Number of permutations of 1..2*n+3 with each element displaced by
 *          at least n.
 * https://oeis.org/A183246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 24*a(n-2) for n > 4 with a(1) = 44, a(2) =
 *            206, a(3) = 1168, a(4) = 6984. The computation uses an on‑demand
 *            cache to allow calls in any order and only depends on basic
 *            integer arithmetic
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but safe */

    /* static cache of previously computed terms */
    static seq_t *cache = NULL;           /* cache[0] = a(1), … */
    static idx_t capacity = 0;            /* allocated capacity */
    static idx_t cached = 0;              /* highest n already computed */

    /* ensure cache capacity */
    if (capacity < n) {
        size_t new_cap = (capacity == 0) ? 4 : capacity * 2;
        while (new_cap < n) new_cap *= 2;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;          /* allocation failed */
        for (idx_t i = 0; i < cached; ++i) new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        capacity = (idx_t)new_cap;
    }

    /* initial values a(1) … a(4) */
    static const seq_t init[4] = {
        (seq_t)44,   /* a(1) */
        (seq_t)206,  /* a(2) */
        (seq_t)1168, /* a(3) */
        (seq_t)6984  /* a(4) */
    };

    /* compute missing terms up to n */
    for (idx_t i = cached + 1; i <= n; ++i) {
        if (i <= 4) {
            cache[i - 1] = init[i - 1];
        } else {
            seq_t t1 = cache[i - 2] * (seq_t)10;  /* 10*a(i-1) */
            seq_t t2 = cache[i - 3] * (seq_t)24;  /* 24*a(i-2) */
            cache[i - 1] = t1 - t2;               /* a(i) = 10*a(i-1) - 24*a(i-2) */
        }
    }
    cached = n;

    return cache[n - 1];
}
