/*
 * A202882  Number of n X 1 0..2 arrays with every nonzero element less than
 *          or equal to some horizontal or vertical neighbor.
 * https://oeis.org/A202882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 4*a(n-3) - a(n-4) + a(n-5);
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 25, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(1)=1, a(2)=3, a(3)=9, a(4)=22, a(5)=51 */

#include <stdlib.h>

/* External types provided by the build system. */
extern seq_t a(idx_t n);

/* Implemented in this file: */
seq_t a(idx_t n)
{
    /* Static cache for previously computed terms. */
    static seq_t *cache = NULL;          /* array of computed terms */
    static size_t cache_cap = 0;          /* allocated capacity, in number of elements */
    static size_t computed = 0;           /* highest index computed so far */

    if (n == 0)  /* not used, but harmless */
        return (seq_t)0;

    /* Initial allocation and seed the first five terms. */
    if (cache == NULL) {
        size_t init_cap = 6;              /* we need at least indices 0..5 */
        if ((size_t)n + 1 > init_cap)
            init_cap = n + 1;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure (unlikely for test sizes) */

        cache_cap = init_cap;
        cache[0] = (seq_t)0;             /* dummy, not used */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)9;
        cache[4] = (seq_t)22;
        cache[5] = (seq_t)51;
        computed = 5;
    }

    /* Grow the cache if we need to address a larger index. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            cache[i] = 3 * cache[i - 1] - 3 * cache[i - 2]
                       + 4 * cache[i - 3] - cache[i - 4] + cache[i - 5];
        }
        computed = n;
    }

    return cache[n];
}
